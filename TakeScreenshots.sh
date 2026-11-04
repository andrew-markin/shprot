#!/bin/bash -e

# Takes README and site screenshots in a VirtualBox VM with Ubuntu GNOME desktop: Preferences
# dialog (Assets/Screenshot-*), server key verification (Assets/HostVerification-*) and
# tray menu with the tunnel open (Assets/TrayMenu-*), one per language and theme, e.g. Assets/Screenshot-Light.png for English and
# Assets/Screenshot-Dark.ru.png for Russian. Image size in READMEs is updated too.
#
# Usage: ./TakeScreenshots.sh [path/to/shprot.deb]
#
# Guest passwords are never needed: the package is unpacked into the guest home
# directory and runs with isolated preferences, so the guest's own Shprot setup
# is not touched. The guest theme is restored on exit. For the tray menu the tunnel is
# really opened: a temporary SSH server runs as the guest user on 127.0.0.1:2022 with
# its own throwaway keys, it only allows port forwarding and is stopped on exit.
#
# GNOME Shell draws the tray menu and takes its direction from the whole session, so the
# tray menu of right-to-left languages is taken in a session with mirrored Shell. To enter
# and leave it the script restarts the VM, so close other guest applications before running.
#
# One-time VM preparation (Ubuntu 26.04 GNOME, Wayland session):
#
# 1. In the guest, install tools and keep the desktop quiet and static:
#      sudo apt install -y openssh-server xdotool python3-gi gir1.2-atspi-2.0 fonts-noto-cjk fonts-noto-core
#      echo "127.0.0.1 myserver.com" | sudo tee -a /etc/hosts
#      gsettings set org.gnome.desktop.screensaver lock-enabled false
#      gsettings set org.gnome.desktop.session idle-delay 0
#      gsettings set org.gnome.desktop.notifications show-banners false
#      gsettings set org.gnome.desktop.interface enable-animations false
#      sudo systemctl disable --now unattended-upgrades
#      sudo sed -i 's/^GRUB_CMDLINE_LINUX_DEFAULT="/&initcall_blacklist=acpi_battery_init /' /etc/default/grub
#      sudo update-grub && sudo reboot
#    and turn on Settings > Users > Automatic Login. The guest's own SSH server plays
#    the demo server myserver.com for the key verification screenshot. The kernel parameter
#    hides the host battery passed by VirtualBox: its charge level would differ in the top bar.
#
#    The host needs ImageMagick and the VirtualBox Python API (vboxapi, installed from
#    the VirtualBox SDK) to click the tray icon: GNOME Shell does not accept synthetic input.
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

def preferences(destination, port, tray):
    return {
        "startAtLogin": tray,
        "showNotifications": tray,
        "sshProxyTunnel": {
            "enabled": True,
            "sshDestination": destination,
            "sshPort": port,
            "sshPrivateKey": open("DemoKey").read(),
            "localSocks5ProxyHost": "",
            "localSocks5ProxyPort": "",
            "localHttpProxyEnabled": tray,
            "localHttpProxyHost": "",
            "localHttpProxyPort": ""
        }
    }

json.dump(preferences("tunnel@myserver.com", "", False), open("Preferences.json", "w"))
json.dump(preferences("$USER@myserver.com", "2022", True), open("TrayPreferences.json", "w"))
EOF

# Temporary SSH server for the tray menu, its host key is trusted in advance
mkdir -m 700 Server
ssh-keygen -q -t ed25519 -N "" -C "" -f Server/HostKey
echo "restrict,port-forwarding $(cat DemoKey.pub)" > Server/AuthorizedKeys
echo "[myserver.com]:2022 $(cut -d" " -f1,2 Server/HostKey.pub)" > Server/KnownHost
cat > Server/sshd_config <<EOF
Port 2022
ListenAddress 127.0.0.1
HostKey $HOME/ShprotScreenshots/Server/HostKey
PidFile $HOME/ShprotScreenshots/Server/sshd.pid
AuthorizedKeysFile $HOME/ShprotScreenshots/Server/AuthorizedKeys
PasswordAuthentication no
KbdInteractiveAuthentication no
UsePAM no
StrictModes no
EOF
rm DemoKey DemoKey.pub'

start_demo_server() {
    guest 'setsid /usr/sbin/sshd -D -e -f $HOME/ShprotScreenshots/Server/sshd_config </dev/null >$HOME/ShprotScreenshots/Server/sshd.log 2>&1 &'
}

start_demo_server

# Restarts the VM from the host, as the guest does not allow rebooting over SSH.
# The power button keeps the shutdown clean, the user is logged in automatically.
restart_guest() {
    VBoxManage controlvm "$VM_NAME" acpipowerbutton

    local stopped=
    for i in $(seq 60); do
        if ! VBoxManage list runningvms | grep -qF "\"$VM_NAME\""; then
            stopped=1
            break
        fi
        sleep 2
    done

    if [ -z "$stopped" ]; then
        echo "ERROR: VM did not shut down, close guest applications and try again."
        exit 1
    fi

    sleep 3 # Let VirtualBox release the VM
    VBoxManage startvm "$VM_NAME" --type gui >/dev/null

    local started=
    for i in $(seq 60); do
        if ssh "${SSH_OPTIONS[@]}" -o ConnectTimeout=3 "$SSH_TARGET" 'pgrep -u $USER -x gnome-shell' >/dev/null 2>&1; then
            started=1
            break
        fi
        sleep 3
    done

    if [ -z "$started" ]; then
        echo "ERROR: Guest session did not start, check automatic login."
        exit 1
    fi

    sleep 15 # Let the session settle, the tray extension starts after GNOME Shell
}

# Remember guest settings and restore them on exit

ORIGINAL_COLOR_SCHEME=$(guest "gsettings get org.gnome.desktop.interface color-scheme")
ORIGINAL_GTK_THEME=$(guest "gsettings get org.gnome.desktop.interface gtk-theme")
ORIGINAL_SHOW_HOME=$(guest "gsettings get org.gnome.shell.extensions.ding show-home")

RTL_SESSION_CONFIG=90-shprot-screenshots.conf
RTL_SESSION=

# Mirrors GNOME Shell through a variable of Clutter, so no guest language packs are needed.
# Home icon of the desktop is hidden, as the screen corner behind the menu gets into the image.
enter_rtl_session() {
    RTL_SESSION=1

    guest "mkdir -p ~/.config/environment.d
           echo CLUTTER_TEXT_DIRECTION=rtl > ~/.config/environment.d/$RTL_SESSION_CONFIG
           gsettings set org.gnome.shell.extensions.ding show-home false"

    restart_guest
    start_demo_server
}

cleanup() {
    guest "~/ShprotScreenshots/Package/opt/Shprot/Shprot --shutdown
           [ -f ~/ShprotScreenshots/Server/sshd.pid ] && kill \$(cat ~/ShprotScreenshots/Server/sshd.pid)
           gsettings set org.gnome.desktop.interface color-scheme $ORIGINAL_COLOR_SCHEME
           gsettings set org.gnome.desktop.interface gtk-theme $ORIGINAL_GTK_THEME
           rm -rf ~/ShprotScreenshots"

    if [ -n "$RTL_SESSION" ]; then
        guest "rm -f ~/.config/environment.d/$RTL_SESSION_CONFIG
               rmdir ~/.config/environment.d 2>/dev/null
               gsettings set org.gnome.shell.extensions.ding show-home $ORIGINAL_SHOW_HOME"

        restart_guest
    fi
}

trap cleanup EXIT

# File suffix and UI language of screenshots, same as README and site languages
LANGUAGES=(":en_US" ".ar:ar_EG" ".fa:fa_IR" ".ru:ru_RU" ".tr:tr_TR" ".vi:vi_VN" ".zh:zh_CN")

is_rtl_language() {
    [ "$1" = ".ar" ] || [ "$1" = ".fa" ]
}

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

# Clicks the guest screen at the given point through VirtualBox
click_guest() {
    python3 - "$VM_NAME" "$1" "$2" <<EOF
import sys, time
from vboxapi import VirtualBoxManager

manager = VirtualBoxManager(None, None)
machine = manager.getVirtualBox().findMachine(sys.argv[1])
session = manager.getSessionObject()
machine.lockMachine(session, manager.constants.LockType_Shared)

try:
    mouse = session.console.mouse
    x, y = int(sys.argv[2]) + 1, int(sys.argv[3]) + 1 # Absolute coordinates are 1-based
    mouse.putMouseEventAbsolute(x, y, 0, 0, 0)
    time.sleep(0.2)
    mouse.putMouseEventAbsolute(x, y, 0, 0, 1)
    time.sleep(0.1)
    mouse.putMouseEventAbsolute(x, y, 0, 0, 0)
finally:
    session.unlockMachine()
EOF
}

# Opens the tray menu and saves it with the top bar part above it to the given file
capture_tray_menu() {
    local output="$1"
    local temporary=$(mktemp -d)

    # The icon is found in the GNOME Shell top bar by the application name
    local icon=$(guest 'python3 - <<EOF
import gi
gi.require_version("Atspi", "2.0")
from gi.repository import Atspi

def find_icon(node):
    if node.get_role_name() == "menu" and node.get_name() == "Shprot":
        return node
    for i in range(node.get_child_count()):
        icon = find_icon(node.get_child_at_index(i))
        if icon:
            return icon
    return None

desktop = Atspi.get_desktop(0)

for i in range(desktop.get_child_count()):
    application = desktop.get_child_at_index(i)
    if application.get_name() == "gnome-shell":
        icon = find_icon(application)
        if icon:
            extents = icon.get_extents(Atspi.CoordType.SCREEN)
            print(extents.x, extents.y, extents.width, extents.height)
EOF')

    if [ -z "$icon" ]; then
        echo "ERROR: Tray icon was not found for $output."
        exit 1
    fi

    read -r iconX iconY iconWidth iconHeight <<< "$icon"

    # GNOME Shell shows the colored icon of the open tunnel a few seconds after the menu says so
    for i in $(seq 30); do
        VBoxManage controlvm "$VM_NAME" screenshotpng "$temporary/Before.png"
        local saturation=$(convert "$temporary/Before.png" -crop "${iconWidth}x$iconHeight+$iconX+$iconY" +repage \
                           -colorspace HSL -channel G -separate -format "%[fx:maxima>0.5]" info:)
        [ "$saturation" = "1" ] && break
        sleep 1
    done

    if [ "$saturation" != "1" ]; then
        echo "ERROR: Tunnel was not opened for $output."
        exit 1
    fi

    click_guest $((iconX + iconWidth / 2)) $((iconY + iconHeight / 2))
    sleep 2
    VBoxManage controlvm "$VM_NAME" screenshotpng "$temporary/After.png"
    VBoxManage controlvm "$VM_NAME" keyboardputscancode 01 81 # Escape closes the menu

    # The menu is the changed area below the top bar
    local width=$(identify -format "%w" "$temporary/After.png")
    local height=$(identify -format "%h" "$temporary/After.png")
    local bar=32
    local menu=$(convert "$temporary/After.png" "$temporary/Before.png" -compose difference -composite \
                 -crop "${width}x$((height - bar))+0+$bar" +repage -colorspace gray -threshold 5% -format "%@" info:)

    local menuWidth=$(echo "$menu" | sed -E 's/^([0-9]+)x.*/\1/')
    local menuHeight=$(echo "$menu" | sed -E 's/^[0-9]+x([0-9]+).*/\1/')
    local menuX=$(echo "$menu" | sed -E 's/^[0-9]+x[0-9]+\+([0-9]+).*/\1/')
    local menuY=$(echo "$menu" | sed -E 's/.*\+([0-9]+)$/\1/')

    if [ "$menuWidth" -lt 100 ] || [ "$menuHeight" -lt 100 ]; then
        echo "ERROR: Tray menu was not opened for $output."
        exit 1
    fi

    local margin=24
    local cropHeight=$((bar + menuY + menuHeight + margin))

    # The image reaches the screen edge where the top bar has the icon: left one in a mirrored session
    if [ "$iconX" -lt $((width / 2)) ]; then
        convert "$temporary/After.png" -crop "$((menuX + menuWidth + margin))x$cropHeight+0+0" +repage "$output"
    else
        local left=$((menuX - margin))
        convert "$temporary/After.png" -crop "$((width - left))x$cropHeight+$left+0" +repage "$output"
    fi

    rm -rf "$temporary"
    echo "$output updated."
}

set_guest_theme() {
    if [ "$1" = "Light" ]; then
        guest "gsettings set org.gnome.desktop.interface color-scheme default
               gsettings set org.gnome.desktop.interface gtk-theme Yaru"
    else
        guest "gsettings set org.gnome.desktop.interface color-scheme prefer-dark
               gsettings set org.gnome.desktop.interface gtk-theme Yaru-dark"
    fi

    sleep 1
}

# Tray menu with the tunnel open through the temporary SSH server
capture_tray() {
    local theme="$1" suffix="$2" locale="$3"

    guest "rm -rf ~/ShprotScreenshots/Config/Shprot && mkdir -p ~/ShprotScreenshots/Config/Shprot/KnownHosts
           cp ~/ShprotScreenshots/TrayPreferences.json ~/ShprotScreenshots/Config/Shprot/Preferences.json
           cp ~/ShprotScreenshots/Server/KnownHost ~/ShprotScreenshots/Config/Shprot/KnownHosts/\$(printf %s myserver.com | sha256sum | cut -d' ' -f1)
           LANGUAGE=$locale LANG=$locale.UTF-8 LC_ALL=$locale.UTF-8 setsid ~/ShprotScreenshots/Package/opt/Shprot/Shprot </dev/null >/dev/null 2>&1 &
           for i in \$(seq 60); do
               ss -ltn | grep -q '127.0.0.1:1080 ' && ss -ltn | grep -q '127.0.0.1:8080 ' && break
               sleep 0.5
           done"

    capture_tray_menu "Assets/TrayMenu-$theme$suffix.png"

    guest "~/ShprotScreenshots/Package/opt/Shprot/Shprot --shutdown"

    sleep 1
}

# Capture dialogs in each theme and language

for THEME in Light Dark; do
    set_guest_theme "$THEME"

    for LANGUAGE_ENTRY in "${LANGUAGES[@]}"; do
        SUFFIX=${LANGUAGE_ENTRY%%:*}
        LOCALE=${LANGUAGE_ENTRY#*:}

        # Fresh preferences without known hosts, so the server key is always unknown
        # Qt takes the language from these variables, so the locale need not be installed in the guest
        guest "rm -rf ~/ShprotScreenshots/Config/Shprot && mkdir -p ~/ShprotScreenshots/Config/Shprot
               cp ~/ShprotScreenshots/Preferences.json ~/ShprotScreenshots/Config/Shprot/Preferences.json
               LANGUAGE=$LOCALE LANG=$LOCALE.UTF-8 LC_ALL=$LOCALE.UTF-8 setsid ~/ShprotScreenshots/Package/opt/Shprot/Shprot --preferences </dev/null >/dev/null 2>&1 &"

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

        if ! is_rtl_language "$SUFFIX"; then
            capture_tray "$THEME" "$SUFFIX" "$LOCALE"
        fi
    done
done

# Tray menu of right-to-left languages in a session with mirrored GNOME Shell

enter_rtl_session

for THEME in Light Dark; do
    set_guest_theme "$THEME"

    for LANGUAGE_ENTRY in "${LANGUAGES[@]}"; do
        SUFFIX=${LANGUAGE_ENTRY%%:*}
        LOCALE=${LANGUAGE_ENTRY#*:}

        if is_rtl_language "$SUFFIX"; then
            capture_tray "$THEME" "$SUFFIX" "$LOCALE"
        fi
    done
done

# Keep image size in READMEs in sync, so pages do not jump while screenshots load

for LANGUAGE_ENTRY in "${LANGUAGES[@]}"; do
    SUFFIX=${LANGUAGE_ENTRY%%:*}

    # Some languages have screenshots for the site only
    [ -f "README$SUFFIX.md" ] || continue

    for IMAGE in Screenshot HostVerification TrayMenu; do
        SIZE=$(file -b "Assets/$IMAGE-Light$SUFFIX.png" | grep -oE '[0-9]+ x [0-9]+')
        WIDTH=${SIZE% x *}
        HEIGHT=${SIZE#* x }

        sed -i -E "s|(src=\"Assets/$IMAGE-Light$SUFFIX.png\" width=\")[0-9]+(\" height=\")[0-9]+|\1$WIDTH\2$HEIGHT|" "README$SUFFIX.md"

        echo "README$SUFFIX.md: $IMAGE size set to ${WIDTH}x${HEIGHT}."
    done
done
