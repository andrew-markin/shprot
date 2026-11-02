---
title: "Shprot — SOCKS5-proxy via SSH die zelf opnieuw verbindt"
description: "Gratis desktopapp voor Windows en Linux die een SOCKS5- en HTTP-proxy via SSH in de lucht houdt: verbindt opnieuw na netwerkwissels en slaapstand, herkent vastgelopen tunnels, zonder opdrachtregel."
readme: "README.md"

ui:
  skip: "Naar de inhoud"
  nav_features: "Functies"
  nav_compare: "Waarom Shprot"
  nav_download: "Downloaden"
  nav_faq: "FAQ"
  language: "Taal"
  github: "Broncode op GitHub"

hero:
  title: "Een SOCKS5-proxy via SSH die verbonden blijft"
  lead: "Shprot maakt van elke server die je via SSH bereikt een stabiele SOCKS5- en HTTP-proxy. De app houdt je netwerk in de gaten, herstelt de tunnel na een wifi-wissel of slaapstand en vertelt je precies wat er misging — zonder opdrachtregel."
  download: "Downloaden"
  download_hint: "Versie {version} voor Windows en Linux"
  facts:
    - "Gratis en open source"
    - "Windows 10 / 11 en Linux"
    - "49 talen"
  screenshot_alt: "Instellingenvenster van Shprot"

features:
  title: "Alles wat een tunnel nodig heeft om te blijven draaien"
  items:
    - id: reconnect
      title: "Overleeft netwerkwissels"
      text: "Ander wifi-netwerk, kabel ingeplugd of laptop uit de slaapstand — Shprot merkt het en herstelt de tunnel vanzelf."
    - id: health
      title: "Vangt stille storingen op"
      text: "Regelmatige controles via de proxy zelf vinden een tunnel die verbonden lijkt maar geen verkeer doorlaat, en starten hem opnieuw."
    - id: gentle
      title: "Vriendelijk voor je server"
      text: "Nieuwe pogingen worden steeds verder uit elkaar gezet en na mislukte aanmeldingen hoogstens eens per minuut herhaald, zodat fail2ban je IP niet blokkeert door een verkeerde sleutel."
    - id: keys
      title: "SSH-sleutels in twee klikken"
      text: "Maak of importeer een sleutel en kopieer het publieke deel voor `authorized_keys` direct vanuit het instellingenvenster."
    - id: trust
      title: "Duidelijke servercontrole"
      text: "Bij de eerste verbinding verschijnt de vingerafdruk van de hostsleutel van de server in een dialoog, zodat je hem kunt controleren voordat je de server vertrouwt."
    - id: http
      title: "Ook een HTTP-proxy"
      text: "Apps zonder SOCKS5-ondersteuning kunnen een optionele HTTP-proxy gebruiken die door dezelfde tunnel loopt."
    - id: tray
      title: "Woont in het systeemvak"
      text: "Het pictogram toont of de tunnel actief is, meldingen leggen uit waarom niet, en de logboeken zijn één klik verwijderd."
    - id: languages
      title: "Spreekt jouw taal"
      text: "De interface is vertaald in 49 talen, met een indeling van rechts naar links voor Arabisch, Perzisch, Hebreeuws en Urdu."

compare:
  title: "Waarom niet gewoon `ssh -D`?"
  lead: "`ssh -D 1080 user@server` geeft je met één opdracht een SOCKS5-proxy — totdat de verbinding ongemerkt stopt met werken. Shprot draait dezelfde OpenSSH-client en regelt alles wat de opdracht alleen niet doet."
  rows:
    - case: "Netwerkwissel, ander wifi-netwerk, ontwaken uit slaapstand"
      ssh: "Zelf opnieuw starten"
      shprot: "Verbindt automatisch opnieuw"
    - case: "Tunnel verbonden maar geen verkeer"
      ssh: "Blijft onopgemerkt"
      shprot: "Gevonden door controles via de proxy"
    - case: "Herhaalde authenticatiefouten"
      ssh: "Aan je eigen scripts"
      shprot: "Zeldzame pogingen, zodat de server je IP niet blokkeert"
    - case: "Clientsleutels"
      ssh: "`ssh-keygen` en bestanden in `~/.ssh`"
      shprot: "Maken of importeren, publieke sleutel in één klik kopiëren"
    - case: "Controle van de serveridentiteit"
      ssh: "Vraag in de terminal"
      shprot: "Dialoog met de vingerafdruk van de hostsleutel"
    - case: "Apps zonder SOCKS5-ondersteuning"
      ssh: "Niet gedekt"
      shprot: "Optionele HTTP-proxy door dezelfde tunnel"
    - case: "Tunnelstatus"
      ssh: "Terminalvenster"
      shprot: "Pictogram in het systeemvak en meldingen met de oorzaak"
  note: "Tools als `autossh` helpen bij herstarten, maar vragen nog steeds handmatige instelling en hebben geen interface."

uses:
  title: "Waar het goed voor is"
  items:
    - id: infra
      title: "Je eigen infrastructuur"
      text: "Bereik interne dashboards, testomgevingen en beheertools die alleen vanuit het netwerk van je server toegankelijk zijn."
    - id: wifi
      title: "Onbetrouwbare netwerken"
      text: "Stuur je verkeer via je eigen server als je openbare wifi gebruikt in cafés, hotels of op luchthavens."
    - id: laptop
      title: "Op afstand werken met een laptop"
      text: "Een stabiele proxy op een computer die de hele dag slaapt en van netwerk wisselt."

how:
  title: "In drie stappen aan de slag"
  steps:
    - title: "Bereid een server voor"
      text: "Elke Linux-server met OpenSSH volstaat. Er wordt niets op geïnstalleerd: een aparte gebruiker die alleen poorten mag doorsturen, maak je met een paar opdrachten aan."
    - title: "Verbind Shprot"
      text: "Vul `user@server` in, maak een sleutel en voeg het publieke deel toe aan `authorized_keys` op de server. Controleer eenmalig de vingerafdruk van de server."
    - title: "Stuur apps naar de proxy"
      text: "Gebruik `socks5h://localhost:1080` in je browser of in de proxyinstellingen van het systeem — het menu in het systeemvak kopieert het adres voor je."
  guide: "Stapsgewijze handleiding met beveiliging van de server"

tour:
  title: "Van dichterbij"
  items:
    - id: trust
      caption: "Bij de eerste verbinding zie je de vingerafdruk van de serversleutel en beslis je of je hem vertrouwt."
    - id: tray
      caption: "Het menu in het systeemvak toont de tunnelstatus en kopieert kant-en-klare proxyadressen."

download:
  title: "Shprot downloaden"
  lead: "Versie {version}. Gratis, open source, zonder accounts of abonnementen."
  windows:
    title: "Windows"
    text: "Windows 10 en 11, 64-bit. Bijwerken doe je door de nieuwe versie over de oude heen te installeren."
    button: "Installatieprogramma (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ en compatibele distributies."
    button: "Pakket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ en compatibele distributies."
    button: "Pakket (.rpm)"
  requirements: "Shprot gebruikt de OpenSSH-client (`ssh` en `ssh-keygen`). Die zit in Windows 10 / 11 en de meeste Linux-desktops; ontbreekt hij, dan legt Shprot uit hoe je hem toevoegt."
  verify: "Elke versie bevat een bestand `SHA256SUMS` waarmee je controleert dat de download niet is gewijzigd."
  all_releases: "Alle versies en wijzigingen"

faq:
  title: "Vragen en antwoorden"
  items:
    - q: "Is Shprot een VPN?"
      a: "Nee, het is een proxy. Alleen apps die ervoor zijn ingesteld — een browser, `curl`, `git`, een IDE — sturen verkeer door de tunnel; al het andere gaat rechtstreeks. Er worden geen virtuele netwerkadapters geïnstalleerd."
    - q: "Wat heb ik nodig op de server?"
      a: "Een gewone OpenSSH-server en een account dat TCP-poorten mag doorsturen. Op de server hoeft niets geïnstalleerd te worden; elke VPS of thuisserver die je via SSH bereikt, volstaat."
    - q: "Kan ik inloggen met een wachtwoord?"
      a: "Nee, Shprot gebruikt alleen SSH-sleutels. Sleutels zijn veiliger dan wachtwoorden en laten de tunnel zonder jouw hulp opnieuw verbinden. De sleutel maak je in de app met één klik."
    - q: "Welke apps kunnen de proxy gebruiken?"
      a: "Browsers, `curl`, `git`, pakketbeheerders en elke app met SOCKS5- of HTTP-proxyinstellingen of ondersteuning voor de variabelen `ALL_PROXY` / `HTTP_PROXY`. Gebruik `socks5h://` zodat sitenamen op de server worden opgezocht."
    - q: "Hoe controleert Shprot de tunnel?"
      a: "Af en toe opent het via de proxy een willekeurig gekozen populaire website. Lukt dat niet terwijl het internet wel bereikbaar is, dan wordt de tunnel opnieuw gestart."
    - q: "Is het gratis?"
      a: "Ja. Shprot is vrije en open source software onder de GNU GPL v3-licentie, zonder accounts, abonnementen of advertenties."
    - q: "Is er een versie voor macOS?"
      a: "Nog niet. Shprot ondersteunt op dit moment Windows 10 / 11 en Linux."

footer:
  license: "Shprot is vrije software onder de GNU GPL v3-licentie."
  source: "Broncode"
  issues: "Een probleem melden"
---
