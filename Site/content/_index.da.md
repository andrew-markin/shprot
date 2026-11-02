---
title: "Shprot — SOCKS5-proxy over SSH, der selv genopretter forbindelsen"
description: "Gratis skrivebordsapp til Windows og Linux, der holder en SOCKS5- og HTTP-proxy over SSH kørende: genopretter forbindelsen efter netværksskift og dvale, opdager tunneller, der hænger, uden kommandolinje."
readme: "README.md"

ui:
  skip: "Gå til indhold"
  nav_features: "Funktioner"
  nav_compare: "Hvorfor Shprot"
  nav_download: "Download"
  nav_faq: "Spørgsmål"
  language: "Sprog"
  github: "Kildekode på GitHub"

hero:
  title: "En SOCKS5-proxy over SSH, der forbliver forbundet"
  lead: "Shprot gør enhver server, du kan nå via SSH, til en stabil SOCKS5- og HTTP-proxy. Den holder øje med dit netværk, genopretter tunnelen efter wi-fi-skift og dvale og fortæller dig præcis, hvad der gik galt — helt uden kommandolinje."
  download: "Download"
  download_hint: "Version {version} til Windows og Linux"
  facts:
    - "Gratis og open source"
    - "Windows 10 / 11 og Linux"
    - "49 sprog"
  screenshot_alt: "Shprots indstillingsvindue"

features:
  title: "Alt, hvad en tunnel behøver for at blive ved med at køre"
  items:
    - id: reconnect
      title: "Klarer netværksskift"
      text: "Skift af wi-fi, et kabel sat i eller en bærbar vækket fra dvale — Shprot opdager det og genopretter selv tunnelen."
    - id: health
      title: "Fanger stille fejl"
      text: "Jævnlige tjek gennem selve proxyen finder en tunnel, der ser forbundet ud, men ikke lukker trafik igennem, og genstarter den."
    - id: gentle
      title: "Skånsom mod din server"
      text: "Nye forsøg spredes gradvist ud, og efter mislykkede logins gentages de højst én gang i minuttet, så fail2ban ikke spærrer din IP på grund af en forkert nøgle."
    - id: keys
      title: "SSH-nøgler med to klik"
      text: "Opret eller importér en nøgle, og kopiér den offentlige del til `authorized_keys` direkte fra indstillingsvinduet."
    - id: trust
      title: "Tydelig kontrol af serveren"
      text: "Ved første forbindelse vises fingeraftrykket af serverens værtsnøgle i en dialog, så du kan tjekke det, før du stoler på serveren."
    - id: http
      title: "Også HTTP-proxy"
      text: "Programmer uden SOCKS5-understøttelse kan bruge en valgfri HTTP-proxy, der går gennem den samme tunnel."
    - id: tray
      title: "Bor i systembakken"
      text: "Ikonet viser, om tunnelen er oppe, notifikationer forklarer hvorfor ikke, og loggen er ét klik væk."
    - id: languages
      title: "Taler dit sprog"
      text: "Brugerfladen er oversat til 49 sprog med højre-til-venstre-layout til arabisk, persisk, hebraisk og urdu."

compare:
  title: "Hvorfor ikke bare `ssh -D`?"
  lead: "`ssh -D 1080 user@server` giver dig en SOCKS5-proxy med én kommando — indtil forbindelsen stille holder op med at virke. Shprot kører den samme OpenSSH-klient og tager sig af alt det, kommandoen ikke gør alene."
  rows:
    - case: "Netværksskift, wi-fi-skift, opvågning fra dvale"
      ssh: "Genstart den selv"
      shprot: "Genopretter forbindelsen automatisk"
    - case: "Tunnel forbundet, men uden trafik"
      ssh: "Går ubemærket hen"
      shprot: "Opdages af tjek gennem proxyen"
    - case: "Gentagne godkendelsesfejl"
      ssh: "Op til dine scripts"
      shprot: "Sjældne forsøg, så serveren ikke spærrer din IP"
    - case: "Klientnøgler"
      ssh: "`ssh-keygen` og filer i `~/.ssh`"
      shprot: "Opret eller importér, kopiér den offentlige nøgle med ét klik"
    - case: "Kontrol af serverens identitet"
      ssh: "Spørgsmål i terminalen"
      shprot: "Dialog med fingeraftrykket af værtsnøglen"
    - case: "Programmer uden SOCKS5-understøttelse"
      ssh: "Ikke dækket"
      shprot: "Valgfri HTTP-proxy gennem samme tunnel"
    - case: "Tunnelstatus"
      ssh: "Terminalvindue"
      shprot: "Ikon i systembakken og notifikationer med årsagen"
  note: "Værktøjer som `autossh` hjælper med genstart, men kræver stadig manuel opsætning og har ingen brugerflade."

uses:
  title: "Hvad den er god til"
  items:
    - id: infra
      title: "Din egen infrastruktur"
      text: "Nå interne dashboards, testmiljøer og administrationsværktøjer, der kun er tilgængelige fra din servers netværk."
    - id: wifi
      title: "Usikre netværk"
      text: "Send din trafik gennem din egen server, når du bruger offentligt wi-fi på caféer, hoteller eller lufthavne."
    - id: laptop
      title: "Fjernarbejde på bærbar"
      text: "En stabil proxy på en computer, der går i dvale og skifter netværk hele dagen."

how:
  title: "Klar i tre trin"
  steps:
    - title: "Klargør en server"
      text: "Enhver Linux-server med OpenSSH duer. Der installeres intet på den — en dedikeret bruger, der kun må videresende porte, oprettes med få kommandoer."
    - title: "Forbind Shprot"
      text: "Indtast `user@server`, opret en nøgle, og tilføj den offentlige del til `authorized_keys` på serveren. Tjek serverens fingeraftryk én gang."
    - title: "Peg programmer mod proxyen"
      text: "Brug `socks5h://localhost:1080` i browseren eller systemets proxyindstillinger — menuen i systembakken kopierer adressen for dig."
  guide: "Trin-for-trin-vejledning med sikring af serveren"

tour:
  title: "Et nærmere kig"
  items:
    - id: trust
      caption: "Ved første forbindelse ser du fingeraftrykket af serverens nøgle og beslutter, om du stoler på den."
    - id: tray
      caption: "Menuen i systembakken viser tunnelens status og kopierer færdige proxyadresser."

download:
  title: "Download Shprot"
  lead: "Version {version}. Gratis, open source, uden konti eller abonnementer."
  windows:
    title: "Windows"
    text: "Windows 10 og 11, 64-bit. Opdatér ved at installere den nye version oven på den gamle."
    button: "Installationsprogram (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ og kompatible distributioner."
    button: "Pakke (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ og kompatible distributioner."
    button: "Pakke (.rpm)"
  requirements: "Shprot bruger OpenSSH-klienten (`ssh` og `ssh-keygen`). Den følger med Windows 10 / 11 og de fleste Linux-skriveborde; mangler den, fortæller Shprot, hvordan du tilføjer den."
  verify: "Hver udgivelse indeholder en `SHA256SUMS`-fil, så du kan tjekke, at downloadet ikke er ændret."
  all_releases: "Alle udgivelser og ændringer"

faq:
  title: "Spørgsmål og svar"
  items:
    - q: "Er Shprot en VPN?"
      a: "Nej, det er en proxy. Kun programmer, der er sat op til at bruge den — en browser, `curl`, `git`, en IDE — sender trafik gennem tunnelen; alt andet går direkte. Der installeres ingen virtuelle netværkskort."
    - q: "Hvad skal jeg bruge på serveren?"
      a: "En almindelig OpenSSH-server og en konto, der må videresende TCP-porte. Der skal ikke installeres noget på serveren; enhver VPS eller hjemmeserver, du kan nå via SSH, kan bruges."
    - q: "Kan jeg logge ind med adgangskode?"
      a: "Nej, Shprot bruger kun SSH-nøgler. Nøgler er mere sikre end adgangskoder og lader tunnelen genoprette forbindelsen uden din hjælp. Nøglen oprettes i appen med ét klik."
    - q: "Hvilke programmer kan bruge proxyen?"
      a: "Browsere, `curl`, `git`, pakkehåndteringer og ethvert program med SOCKS5- eller HTTP-proxyindstillinger eller understøttelse af variablerne `ALL_PROXY` / `HTTP_PROXY`. Brug `socks5h://`, så webstedsnavne slås op på serveren."
    - q: "Hvordan tjekker Shprot tunnelen?"
      a: "Med jævne mellemrum åbner den et tilfældigt valgt populært websted gennem proxyen. Lykkes det ikke, selvom internettet er tilgængeligt, genstartes tunnelen."
    - q: "Er det gratis?"
      a: "Ja. Shprot er fri og open source-software under GNU GPL v3-licensen, uden konti, abonnementer eller reklamer."
    - q: "Findes der en macOS-version?"
      a: "Ikke endnu. Shprot understøtter i dag Windows 10 / 11 og Linux."

footer:
  license: "Shprot er fri software under GNU GPL v3-licensen."
  source: "Kildekode"
  issues: "Rapportér et problem"
---
