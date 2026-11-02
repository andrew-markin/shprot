#!/bin/bash -e

# Takes README screenshots in a VirtualBox VM with Ubuntu GNOME desktop: Preferences
# dialog (Assets/Screenshot-*) and server key verification (Assets/HostVerification-*),
# one per README language and theme, e.g. Assets/Screenshot-Light.png for README.md and
# Assets/Screenshot-Dark.ru.png for README.ru.md. Image size in READMEs is updated too.
#
# Usage: ./TakeScreenshots.sh [path/to/shprot.deb]
#
# Guest passwords are never needed: the package is unpacked into the guest home
# directory and runs with isolated preferences, so the guest's own Shprot setup
# is not touched. The guest theme is restored on exit.
#
# One-time VM preparation (Ubuntu 26.04 GNOME, Wayland session):
#
# 1. In the guest, install tools and keep the desktop quiet and static:
#      sudo apt install -y openssh-server xdotool python3-gi gir1.2-atspi-2.0 fonts-noto-cjk
#      echo "127.0.0.1 myserver.com" | sudo tee -a /etc/hosts
#      gsettings set org.gnome.desktop.screensaver lock-enabled false
#      gsettings set org.gnome.desktop.session idle-delay 0
#      gsettings set org.gnome.desktop.notifications show-banners false
#      gsettings set org.gnome.desktop.interface enable-animations false
#      sudo systemctl disable --now unattended-upgrades
#    and turn on Settings > Users > Automatic Login. The guest's own SSH server plays
#    the demo server myserver.com for the key verification screenshot.
#
# 2. On the host, create a key for the VM and forward SSH to the guest
#    (controlvm works on a running VM, use modifyvm --natpf1 on a stopped one):
#      ssh-keygen -t ed25519 -N "" -f ~/.ssh/shprot_vm_ed25519
#      VBoxManage controlvm "Ubuntu Development" natpf1 "ssh,tcp,127.0.0.1,2222,,22"
#
# 3. In the guest, authorize the key (copy the .pub file via a shared folder or USB):
#      mkdir -p ~/.ssh && chmod 700 ~/.ssh
#      cat shprot_vm_ed25519.pub >> ~/.ssh/authorized_keys && chmod 600 ~/.ssh/authorized_keys
#
# 4. Check from the host:
#      ssh -i ~/.ssh/shprot_vm_ed25519 -p 2222 developer@127.0.0.1 true
#
# The VM must be running with the user logged in. Other VM names, users, ports
# or keys are set in .env (see .env.example).

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Load environment variables from .env file if exists
if [ -f ".env" ]; then
    set -a  # Automatically export all variables
    source .env
    set +a
fi

VM_NAME="${SCREENSHOTS_VM:-Ubuntu Development}"
SSH_TARGET="${SCREENSHOTS_SSH_TARGET:-developer@127.0.0.1}"
SSH_PORT="${SCREENSHOTS_SSH_PORT:-2222}"
SSH_KEY="${SCREENSHOTS_SSH_KEY:-$HOME/.ssh/shprot_vm_ed25519}"

PACKAGE="$1"

if [ -z "$PACKAGE" ]; then
    PACKAGE=$(ls -t Release/shprot-*.deb 2>/dev/null | head -1)
fi

if [ ! -f "$PACKAGE" ]; then
    echo "ERROR: DEB package not found. Run ./ReleaseUbuntu.sh or pass the package path."
    exit 1
fi

SSH_OPTIONS=(-i "$SSH_KEY" -p "$SSH_PORT" -o BatchMode=yes)
SCP_OPTIONS=(-q -i "$SSH_KEY" -P "$SSH_PORT" -o BatchMode=yes)

# Runs a command in the guest with the environment of its GNOME session
guest() {
    ssh "${SSH_OPTIONS[@]}" "$SSH_TARGET" "
        while IFS= read -r -d '' v; do export \"\$v\"; done < /proc/\$(pgrep -u \$USER -x gnome-shell | head -1)/environ
        export DISPLAY=:0 XAUTHORITY=\$(ls /run/user/\$(id -u)/.mutter-Xwaylandauth.* | head -1)
        export XDG_CONFIG_HOME=~/ShprotScreenshots/Config XDG_DATA_HOME=~/ShprotScreenshots/Data XDG_CACHE_HOME=~/ShprotScreenshots/Cache
        $1"
}

# Prepare the application and demo preferences in the guest. Preferences are copied
# before each run, so dialog geometry saved by one run does not affect another.

guest "rm -rf ~/ShprotScreenshots && mkdir -p ~/ShprotScreenshots/Config/Shprot ~/ShprotScreenshots/Package"
scp "${SCP_OPTIONS[@]}" "$PACKAGE" "$SSH_TARGET:ShprotScreenshots/shprot.deb"

guest 'cd ~/ShprotScreenshots
dpkg-deb -x shprot.deb Package
ssh-keygen -q -t ed25519 -N "" -C "" -f DemoKey
python3 - <<EOF
import json
json.dump({
    "startAtLogin": False,
    "showNotifications": False,
    "sshProxyTunnel": {
        "enabled": True,
        "sshDestination": "tunnel@myserver.com",
        "sshPort": "",
        "sshPrivateKey": open("DemoKey").read(),
        "localSocks5ProxyHost": "",
        "localSocks5ProxyPort": "",
        "localHttpProxyEnabled": False,
        "localHttpProxyHost": "",
        "localHttpProxyPort": ""
    }
}, open("Preferences.json", "w"))
EOF
rm DemoKey DemoKey.pub'

# Remember the guest theme and restore it on exit

ORIGINAL_COLOR_SCHEME=$(guest "gsettings get org.gnome.desktop.interface color-scheme")
ORIGINAL_GTK_THEME=$(guest "gsettings get org.gnome.desktop.interface gtk-theme")

cleanup() {
    guest "~/ShprotScreenshots/Package/opt/Shprot/Shprot --shutdown
           gsettings set org.gnome.desktop.interface color-scheme $ORIGINAL_COLOR_SCHEME
           gsettings set org.gnome.desktop.interface gtk-theme $ORIGINAL_GTK_THEME
           rm -rf ~/ShprotScreenshots"
}

trap cleanup EXIT

# README file suffix and UI language of its screenshots
LANGUAGES=(":en_US" ".ru:ru_RU" ".zh:zh_CN")

# Without it the key verification shows another host name or fails
if ! guest "getent hosts myserver.com | grep -qE '^(127\.|::1)'"; then
    echo "ERROR: myserver.com does not resolve to the guest itself, see VM preparation at the top of this script."
    exit 1
fi

# Saves the focused guest window to the given file
capture_window() {
    local output="$1"

    PREVIOUS=$(guest "ls -t ~/Pictures/Screenshots/ 2>/dev/null | head -1")

    # Alt+Print: GNOME saves the focused window with its frame and transparent shadow
    VBoxManage controlvm "$VM_NAME" keyboardputscancode 38 54 d4 b8

    for i in $(seq 10); do
        sleep 1
        SCREENSHOT=$(guest "ls -t ~/Pictures/Screenshots/ 2>/dev/null | head -1")
        [ "$SCREENSHOT" != "$PREVIOUS" ] && break
    done

    if [ "$SCREENSHOT" = "$PREVIOUS" ]; then
        echo "ERROR: $output was not taken."
        exit 1
    fi

    sleep 1 # Let GNOME finish writing the file

    scp "${SCP_OPTIONS[@]}" "$SSH_TARGET:Pictures/Screenshots/$SCREENSHOT" "$output"
    guest "rm -f ~/Pictures/Screenshots/'$SCREENSHOT'"

    echo "$output updated."
}

# Capture dialogs in each theme and language

for THEME in Light Dark; do
    if [ "$THEME" = "Light" ]; then
        guest "gsettings set org.gnome.desktop.interface color-scheme default
               gsettings set org.gnome.desktop.interface gtk-theme Yaru"
    else
        guest "gsettings set org.gnome.desktop.interface color-scheme prefer-dark
               gsettings set org.gnome.desktop.interface gtk-theme Yaru-dark"
    fi

    sleep 1

    for LANGUAGE_ENTRY in "${LANGUAGES[@]}"; do
        SUFFIX=${LANGUAGE_ENTRY%%:*}
        LOCALE=${LANGUAGE_ENTRY#*:}

        # Fresh preferences without known hosts, so the server key is always unknown
        guest "rm -rf ~/ShprotScreenshots/Config/Shprot && mkdir -p ~/ShprotScreenshots/Config/Shprot
               cp ~/ShprotScreenshots/Preferences.json ~/ShprotScreenshots/Config/Shprot/Preferences.json
               export LANGUAGE=$LOCALE LANG=$LOCALE.UTF-8 LC_ALL=$LOCALE.UTF-8
               setsid ~/ShprotScreenshots/Package/opt/Shprot/Shprot --preferences </dev/null >/dev/null 2>&1 &"

        # Window title is translated, so the dialog is found by its class
        guest 'w=$(xdotool search --sync --onlyvisible --class Shprot | head -1)
               xdotool windowactivate --sync $w 2>/dev/null || true
               xdotool mousemove 0 0' # Keep the pointer away from the dialog

        sleep 2

        capture_window "Assets/Screenshot-$THEME$SUFFIX.png"

        # Button texts are translated too, so Run Trust Check is pressed through accessibility
        # as the first button of the Preferences dialog
        guest 'python3 - <<EOF
import gi
gi.require_version("Atspi", "2.0")
from gi.repository import Atspi

def find_button(node):
    if node.get_role_name() == "button":
        return node
    for i in range(node.get_child_count()):
        button = find_button(node.get_child_at_index(i))
        if button:
            return button
    return None

desktop = Atspi.get_desktop(0)

for i in range(desktop.get_child_count()):
    application = desktop.get_child_at_index(i)
    if application.get_name() == "Shprot":
        button = find_button(application)
        if button:
            button.get_action_iface().do_action(0)
            break
EOF
               for i in $(seq 20); do
                   [ $(xdotool search --onlyvisible --class Shprot | wc -l) -ge 2 ] && break
                   sleep 0.5
               done
               xdotool mousemove 0 0'

        sleep 2

        capture_window "Assets/HostVerification-$THEME$SUFFIX.png"

        guest "~/ShprotScreenshots/Package/opt/Shprot/Shprot --shutdown"

        sleep 1
    done
done

# Keep image size in READMEs in sync, so pages do not jump while screenshots load

for LANGUAGE_ENTRY in "${LANGUAGES[@]}"; do
    SUFFIX=${LANGUAGE_ENTRY%%:*}

    for IMAGE in Screenshot HostVerification; do
        SIZE=$(file -b "Assets/$IMAGE-Light$SUFFIX.png" | grep -oE '[0-9]+ x [0-9]+')
        WIDTH=${SIZE% x *}
        HEIGHT=${SIZE#* x }

        sed -i -E "s|(src=\"Assets/$IMAGE-Light$SUFFIX.png\" width=\")[0-9]+(\" height=\")[0-9]+|\1$WIDTH\2$HEIGHT|" "README$SUFFIX.md"

        echo "README$SUFFIX.md: $IMAGE size set to ${WIDTH}x${HEIGHT}."
    done
done
