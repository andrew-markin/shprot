---
# The text of the page. Every language has the same structure, "id" values are not translated.
# Texts may contain inline Markdown, {version} is replaced with the latest release version.
title: "Shprot — SOCKS5 proxy over SSH that reconnects by itself"
description: "Free desktop app for Windows and Linux that keeps a SOCKS5 and HTTP proxy over SSH alive: reconnects after network changes and sleep, detects stalled tunnels, no command line."
readme: "README.md"

ui:
  skip: "Skip to content"
  nav_features: "Features"
  nav_compare: "Why Shprot"
  nav_download: "Download"
  nav_faq: "FAQ"
  language: "Language"
  github: "Source code on GitHub"

hero:
  title: "A SOCKS5 proxy over SSH that stays connected"
  lead: "Shprot turns any server you can reach over SSH into a stable SOCKS5 and HTTP proxy. It watches your network, brings the tunnel back after Wi-Fi changes and sleep, and tells you exactly what went wrong — no command line needed."
  download: "Download"
  download_hint: "Version {version} for Windows and Linux"
  facts:
    - "Free and open source"
    - "Windows 10 / 11 and Linux"
    - "7 languages"
  screenshot_alt: "Shprot settings window"

features:
  title: "Everything a tunnel needs to keep running"
  items:
    - id: reconnect
      title: "Survives network changes"
      text: "Switching Wi-Fi, plugging in a cable or waking the laptop from sleep — Shprot notices and brings the tunnel back on its own."
    - id: health
      title: "Catches silent failures"
      text: "Regular checks through the proxy itself detect a tunnel that looks connected but passes no traffic, and restart it."
    - id: gentle
      title: "Kind to your server"
      text: "Retries back off gradually, and failed logins are repeated at most once a minute, so fail2ban won't ban your IP over a wrong key."
    - id: keys
      title: "SSH keys in two clicks"
      text: "Generate or import a key and copy the public key for `authorized_keys` right from the settings window."
    - id: trust
      title: "Clear server verification"
      text: "On the first connection the server's host key fingerprint is shown in a dialog, so you can check it before trusting the server."
    - id: http
      title: "HTTP proxy, too"
      text: "Apps that don't support SOCKS5 can use an optional HTTP proxy that goes through the same tunnel."
    - id: tray
      title: "Lives in the tray"
      text: "The icon shows whether the tunnel is up, notifications explain why it isn't, and logs are one click away."
    - id: languages
      title: "Speaks your language"
      text: "The interface is available in 7 languages, with right-to-left layout for Arabic and Persian."

compare:
  title: "Why not just `ssh -D`?"
  lead: "`ssh -D 1080 user@server` gives you a SOCKS5 proxy in one command — until the connection silently stops working. Shprot runs the same OpenSSH client and takes care of everything the command alone doesn't."
  rows:
    - case: "Network change, Wi-Fi switch, wake from sleep"
      ssh: "Restart it yourself"
      shprot: "Reconnects automatically"
    - case: "Tunnel connected but not passing traffic"
      ssh: "Goes unnoticed"
      shprot: "Detected by health checks through the proxy"
    - case: "Repeated authentication failures"
      ssh: "Up to your scripts"
      shprot: "Rare retries, so the server doesn't ban your IP"
    - case: "Client keys"
      ssh: "`ssh-keygen` and files in `~/.ssh`"
      shprot: "Generate or import, copy the public key in one click"
    - case: "Server identity check"
      ssh: "Terminal prompt"
      shprot: "Dialog with the host key fingerprint"
    - case: "Apps without SOCKS5 support"
      ssh: "Not covered"
      shprot: "Optional HTTP proxy through the same tunnel"
    - case: "Tunnel status"
      ssh: "Terminal window"
      shprot: "Tray icon and notifications with the failure reason"
  note: "Tools like `autossh` help with restarts, but still need manual setup and have no user interface."

uses:
  title: "What it's good for"
  items:
    - id: infra
      title: "Your own infrastructure"
      text: "Reach internal dashboards, staging environments and admin tools that are only accessible from your server's network."
    - id: wifi
      title: "Untrusted networks"
      text: "Send your traffic through your own server when using public Wi-Fi in cafés, hotels or airports."
    - id: laptop
      title: "Remote work on a laptop"
      text: "Keep a stable proxy on a machine that sleeps and changes networks all day."

how:
  title: "Up and running in three steps"
  steps:
    - title: "Prepare a server"
      text: "Any Linux server with OpenSSH will do. Nothing is installed on it — a dedicated user that can only forward ports takes a few commands."
    - title: "Connect Shprot"
      text: "Enter `user@server`, generate a key and add its public part to the server's `authorized_keys`. Check the server fingerprint once."
    - title: "Point apps to the proxy"
      text: "Use `socks5h://localhost:1080` in your browser or system proxy settings — the tray menu copies the address for you."
  guide: "Step-by-step setup guide with server hardening"

tour:
  title: "A closer look"
  items:
    - id: trust
      caption: "On the first connection you see the server's key fingerprint and decide whether to trust it."
    - id: tray
      caption: "The tray menu shows the tunnel state and copies ready-to-use proxy addresses."

download:
  title: "Download Shprot"
  lead: "Version {version}. Free, open source, no accounts or subscriptions."
  windows:
    title: "Windows"
    text: "Windows 10 and 11, 64-bit. To update, install a newer version over the old one."
    button: "Installer (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ and compatible distributions."
    button: "Package (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ and compatible distributions."
    button: "Package (.rpm)"
  requirements: "Shprot uses the OpenSSH client (`ssh` and `ssh-keygen`). It comes with Windows 10 / 11 and most Linux desktops; if it's missing, Shprot will tell you how to add it."
  verify: "Every release includes a `SHA256SUMS` file to check that the download wasn't altered."
  all_releases: "All releases and changes"

faq:
  title: "Questions and answers"
  items:
    - q: "Is Shprot a VPN?"
      a: "No, it's a proxy. Only apps that are set up to use it — a browser, `curl`, `git`, an IDE — send traffic through the tunnel; everything else goes directly. No virtual network adapters are installed."
    - q: "What do I need on the server?"
      a: "A regular OpenSSH server and an account that is allowed to forward TCP ports. Nothing has to be installed on the server; any VPS or home server you can reach over SSH will do."
    - q: "Can I log in with a password?"
      a: "No, Shprot uses SSH keys only. Keys are safer than passwords and let the tunnel reconnect unattended. The key can be generated in the app in one click."
    - q: "Which apps can use the proxy?"
      a: "Browsers, `curl`, `git`, package managers and any app with SOCKS5 or HTTP proxy settings or support for the `ALL_PROXY` / `HTTP_PROXY` variables. Use `socks5h://` so that site names are resolved on the server."
    - q: "How does Shprot check the tunnel?"
      a: "From time to time it opens a randomly chosen popular website through the proxy. If that fails while the internet itself is available, the tunnel is restarted."
    - q: "Is it free?"
      a: "Yes. Shprot is free and open source software under the GNU GPL v3 license, with no accounts, subscriptions or ads."
    - q: "Is there a macOS version?"
      a: "Not yet. Shprot currently supports Windows 10 / 11 and Linux."

footer:
  license: "Shprot is free software licensed under the GNU GPL v3."
  source: "Source code"
  issues: "Report a problem"
---
