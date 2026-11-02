---
title: "Shprot — SOCKS5-Proxy über SSH, der sich selbst neu verbindet"
description: "Kostenlose Desktop-App für Windows und Linux, die einen SOCKS5- und HTTP-Proxy über SSH am Laufen hält: verbindet sich nach Netzwechsel und Ruhezustand neu, erkennt hängende Tunnel, ohne Kommandozeile."
readme: "README.md"

ui:
  skip: "Zum Inhalt springen"
  nav_features: "Funktionen"
  nav_compare: "Warum Shprot"
  nav_download: "Download"
  nav_faq: "FAQ"
  language: "Sprache"
  github: "Quellcode auf GitHub"

hero:
  title: "Ein SOCKS5-Proxy über SSH, der verbunden bleibt"
  lead: "Shprot macht aus jedem Server, den Sie per SSH erreichen, einen stabilen SOCKS5- und HTTP-Proxy. Die App beobachtet Ihr Netzwerk, stellt den Tunnel nach WLAN-Wechsel und Ruhezustand wieder her und sagt Ihnen genau, was schiefgelaufen ist — ganz ohne Kommandozeile."
  download: "Herunterladen"
  download_hint: "Version {version} für Windows und Linux"
  facts:
    - "Kostenlos und Open Source"
    - "Windows 10 / 11 und Linux"
    - "49 Sprachen"
  screenshot_alt: "Einstellungsfenster von Shprot"

features:
  title: "Alles, was ein Tunnel zum Durchhalten braucht"
  items:
    - id: reconnect
      title: "Übersteht Netzwechsel"
      text: "WLAN gewechselt, Kabel eingesteckt oder Laptop aus dem Ruhezustand geweckt — Shprot bemerkt es und stellt den Tunnel selbstständig wieder her."
    - id: health
      title: "Erkennt stille Ausfälle"
      text: "Regelmäßige Prüfungen über den Proxy selbst finden einen Tunnel, der verbunden aussieht, aber keinen Verkehr durchlässt, und starten ihn neu."
    - id: gentle
      title: "Schont Ihren Server"
      text: "Die Abstände zwischen Versuchen wachsen schrittweise, und nach fehlgeschlagenen Anmeldungen wird höchstens einmal pro Minute wiederholt — fail2ban sperrt Ihre IP also nicht wegen eines falschen Schlüssels."
    - id: keys
      title: "SSH-Schlüssel mit zwei Klicks"
      text: "Erzeugen oder importieren Sie einen Schlüssel und kopieren Sie den öffentlichen Teil für `authorized_keys` direkt aus dem Einstellungsfenster."
    - id: trust
      title: "Klare Serverprüfung"
      text: "Bei der ersten Verbindung wird der Fingerabdruck des Server-Hostschlüssels in einem Dialog angezeigt, damit Sie ihn prüfen können, bevor Sie dem Server vertrauen."
    - id: http
      title: "Auch ein HTTP-Proxy"
      text: "Programme ohne SOCKS5-Unterstützung können einen optionalen HTTP-Proxy nutzen, der durch denselben Tunnel läuft."
    - id: tray
      title: "Lebt im Infobereich"
      text: "Das Symbol zeigt, ob der Tunnel steht, Benachrichtigungen erklären, warum nicht, und das Protokoll ist einen Klick entfernt."
    - id: languages
      title: "Spricht Ihre Sprache"
      text: "Die Oberfläche ist in 49 Sprachen übersetzt, mit Rechts-nach-links-Layout für Arabisch, Persisch, Hebräisch und Urdu."

compare:
  title: "Warum nicht einfach `ssh -D`?"
  lead: "`ssh -D 1080 user@server` liefert mit einem Befehl einen SOCKS5-Proxy — bis die Verbindung unbemerkt nicht mehr funktioniert. Shprot startet denselben OpenSSH-Client und kümmert sich um alles, was der Befehl allein nicht leistet."
  rows:
    - case: "Netzwechsel, WLAN-Wechsel, Aufwachen aus dem Ruhezustand"
      ssh: "Selbst neu starten"
      shprot: "Verbindet sich automatisch neu"
    - case: "Tunnel verbunden, aber kein Datenverkehr"
      ssh: "Bleibt unbemerkt"
      shprot: "Wird durch Prüfungen über den Proxy erkannt"
    - case: "Wiederholte Authentifizierungsfehler"
      ssh: "Sache Ihrer Skripte"
      shprot: "Seltene Wiederholungen, damit der Server Ihre IP nicht sperrt"
    - case: "Client-Schlüssel"
      ssh: "`ssh-keygen` und Dateien in `~/.ssh`"
      shprot: "Erzeugen oder importieren, öffentlichen Schlüssel mit einem Klick kopieren"
    - case: "Prüfung der Serveridentität"
      ssh: "Abfrage im Terminal"
      shprot: "Dialog mit dem Fingerabdruck des Hostschlüssels"
    - case: "Programme ohne SOCKS5-Unterstützung"
      ssh: "Nicht abgedeckt"
      shprot: "Optionaler HTTP-Proxy durch denselben Tunnel"
    - case: "Tunnelstatus"
      ssh: "Terminalfenster"
      shprot: "Symbol im Infobereich und Benachrichtigungen mit dem Fehlergrund"
  note: "Werkzeuge wie `autossh` helfen beim Neustart, erfordern aber trotzdem manuelle Einrichtung und haben keine Oberfläche."

uses:
  title: "Wofür es sich eignet"
  items:
    - id: infra
      title: "Eigene Infrastruktur"
      text: "Erreichen Sie interne Dashboards, Staging-Umgebungen und Admin-Werkzeuge, die nur aus dem Netz Ihres Servers zugänglich sind."
    - id: wifi
      title: "Unsichere Netzwerke"
      text: "Leiten Sie Ihren Verkehr über Ihren eigenen Server, wenn Sie öffentliches WLAN in Cafés, Hotels oder Flughäfen nutzen."
    - id: laptop
      title: "Mobiles Arbeiten am Laptop"
      text: "Ein stabiler Proxy auf einem Rechner, der den ganzen Tag schläft und das Netz wechselt."

how:
  title: "In drei Schritten startklar"
  steps:
    - title: "Server vorbereiten"
      text: "Jeder Linux-Server mit OpenSSH genügt. Installiert wird darauf nichts — ein eigener Benutzer, der nur Ports weiterleiten darf, ist mit wenigen Befehlen angelegt."
    - title: "Shprot verbinden"
      text: "Geben Sie `user@server` ein, erzeugen Sie einen Schlüssel und fügen Sie dessen öffentlichen Teil in `authorized_keys` auf dem Server ein. Prüfen Sie einmal den Fingerabdruck des Servers."
    - title: "Programme auf den Proxy umstellen"
      text: "Tragen Sie `socks5h://localhost:1080` im Browser oder in den Proxy-Einstellungen des Systems ein — das Menü im Infobereich kopiert die Adresse für Sie."
  guide: "Schritt-für-Schritt-Anleitung mit Serverhärtung"

tour:
  title: "Ein genauerer Blick"
  items:
    - id: trust
      caption: "Bei der ersten Verbindung sehen Sie den Fingerabdruck des Serverschlüssels und entscheiden, ob Sie ihm vertrauen."
    - id: tray
      caption: "Das Menü im Infobereich zeigt den Tunnelstatus und kopiert fertige Proxy-Adressen."

download:
  title: "Shprot herunterladen"
  lead: "Version {version}. Kostenlos, Open Source, ohne Konten und Abonnements."
  windows:
    title: "Windows"
    text: "Windows 10 und 11, 64 Bit. Zum Aktualisieren einfach die neue Version über die alte installieren."
    button: "Installationsprogramm (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ und kompatible Distributionen."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ und kompatible Distributionen."
    button: "Paket (.rpm)"
  requirements: "Shprot nutzt den OpenSSH-Client (`ssh` und `ssh-keygen`). Er ist in Windows 10 / 11 und den meisten Linux-Desktops enthalten; fehlt er, erklärt Shprot, wie Sie ihn hinzufügen."
  verify: "Jede Version enthält eine Datei `SHA256SUMS`, mit der Sie prüfen können, dass der Download nicht verändert wurde."
  all_releases: "Alle Versionen und Änderungen"

faq:
  title: "Fragen und Antworten"
  items:
    - q: "Ist Shprot ein VPN?"
      a: "Nein, es ist ein Proxy. Nur Programme, die dafür eingerichtet sind — ein Browser, `curl`, `git`, eine IDE —, senden Verkehr durch den Tunnel; alles andere geht direkt. Es werden keine virtuellen Netzwerkadapter installiert."
    - q: "Was brauche ich auf dem Server?"
      a: "Einen normalen OpenSSH-Server und ein Konto, das TCP-Ports weiterleiten darf. Auf dem Server muss nichts installiert werden; jeder VPS oder Heimserver, den Sie per SSH erreichen, eignet sich."
    - q: "Kann ich mich mit einem Passwort anmelden?"
      a: "Nein, Shprot verwendet nur SSH-Schlüssel. Schlüssel sind sicherer als Passwörter und erlauben es dem Tunnel, sich ohne Ihr Zutun neu zu verbinden. Der Schlüssel wird in der App mit einem Klick erzeugt."
    - q: "Welche Programme können den Proxy nutzen?"
      a: "Browser, `curl`, `git`, Paketmanager und jedes Programm mit SOCKS5- oder HTTP-Proxy-Einstellungen oder Unterstützung für die Variablen `ALL_PROXY` / `HTTP_PROXY`. Verwenden Sie `socks5h://`, damit Domainnamen auf dem Server aufgelöst werden."
    - q: "Wie prüft Shprot den Tunnel?"
      a: "Von Zeit zu Zeit öffnet es über den Proxy eine zufällig gewählte bekannte Website. Schlägt das fehl, obwohl das Internet selbst erreichbar ist, wird der Tunnel neu gestartet."
    - q: "Ist es kostenlos?"
      a: "Ja. Shprot ist freie Open-Source-Software unter der Lizenz GNU GPL v3, ohne Konten, Abonnements oder Werbung."
    - q: "Gibt es eine macOS-Version?"
      a: "Noch nicht. Shprot unterstützt derzeit Windows 10 / 11 und Linux."

footer:
  license: "Shprot ist freie Software unter der Lizenz GNU GPL v3."
  source: "Quellcode"
  issues: "Problem melden"
---
