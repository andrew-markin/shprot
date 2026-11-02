---
title: "Shprot — SOCKS5-proxy over SSH som kobler til på nytt av seg selv"
description: "Gratis skrivebordsapp for Windows og Linux som holder en SOCKS5- og HTTP-proxy over SSH i gang: kobler til på nytt etter nettverksbytte og dvale, oppdager tunneler som har hengt seg, uten kommandolinje."
readme: "README.md"

ui:
  skip: "Gå til innhold"
  nav_features: "Funksjoner"
  nav_compare: "Hvorfor Shprot"
  nav_download: "Last ned"
  nav_faq: "Spørsmål"
  language: "Språk"
  github: "Kildekode på GitHub"

hero:
  title: "En SOCKS5-proxy over SSH som holder seg tilkoblet"
  lead: "Shprot gjør enhver server du når via SSH, om til en stabil SOCKS5- og HTTP-proxy. Den følger med på nettverket, gjenoppretter tunnelen etter bytte av wifi og dvale og forteller deg nøyaktig hva som gikk galt — uten kommandolinje."
  download: "Last ned"
  download_hint: "Versjon {version} for Windows og Linux"
  facts:
    - "Gratis og åpen kildekode"
    - "Windows 10 / 11 og Linux"
    - "49 språk"
  screenshot_alt: "Innstillingsvinduet i Shprot"

features:
  title: "Alt en tunnel trenger for å holde seg i gang"
  items:
    - id: reconnect
      title: "Tåler nettverksbytte"
      text: "Nytt wifi-nett, en kabel som kobles til eller en bærbar som vekkes fra dvale — Shprot merker det og gjenoppretter tunnelen selv."
    - id: health
      title: "Fanger stille feil"
      text: "Jevnlige sjekker gjennom selve proxyen finner en tunnel som ser tilkoblet ut, men ikke slipper gjennom trafikk, og starter den på nytt."
    - id: gentle
      title: "Skånsom mot serveren"
      text: "Nye forsøk spres gradvis ut, og etter mislykkede pålogginger gjentas de høyst én gang i minuttet, så fail2ban ikke blokkerer IP-en din på grunn av feil nøkkel."
    - id: keys
      title: "SSH-nøkler med to klikk"
      text: "Lag eller importer en nøkkel, og kopier den offentlige delen til `authorized_keys` rett fra innstillingsvinduet."
    - id: trust
      title: "Tydelig serverkontroll"
      text: "Ved første tilkobling vises fingeravtrykket til serverens vertsnøkkel i en dialog, slik at du kan sjekke det før du stoler på serveren."
    - id: http
      title: "HTTP-proxy også"
      text: "Programmer uten støtte for SOCKS5 kan bruke en valgfri HTTP-proxy som går gjennom den samme tunnelen."
    - id: tray
      title: "Bor i systemstatusfeltet"
      text: "Ikonet viser om tunnelen er oppe, varsler forklarer hvorfor ikke, og loggen er ett klikk unna."
    - id: languages
      title: "Snakker ditt språk"
      text: "Grensesnittet er oversatt til 49 språk, med høyre-til-venstre-oppsett for arabisk, persisk, hebraisk og urdu."

compare:
  title: "Hvorfor ikke bare `ssh -D`?"
  lead: "`ssh -D 1080 user@server` gir deg en SOCKS5-proxy med én kommando — helt til forbindelsen i stillhet slutter å virke. Shprot kjører den samme OpenSSH-klienten og tar seg av alt kommandoen ikke gjør alene."
  rows:
    - case: "Nettverksbytte, bytte av wifi, oppvåkning fra dvale"
      ssh: "Start den på nytt selv"
      shprot: "Kobler til på nytt automatisk"
    - case: "Tunnelen er tilkoblet, men uten trafikk"
      ssh: "Går upåaktet hen"
      shprot: "Oppdages av sjekker gjennom proxyen"
    - case: "Gjentatte autentiseringsfeil"
      ssh: "Opp til skriptene dine"
      shprot: "Sjeldne forsøk, så serveren ikke blokkerer IP-en din"
    - case: "Klientnøkler"
      ssh: "`ssh-keygen` og filer i `~/.ssh`"
      shprot: "Lag eller importer, kopier den offentlige nøkkelen med ett klikk"
    - case: "Kontroll av serverens identitet"
      ssh: "Spørsmål i terminalen"
      shprot: "Dialog med fingeravtrykket til vertsnøkkelen"
    - case: "Programmer uten støtte for SOCKS5"
      ssh: "Ikke dekket"
      shprot: "Valgfri HTTP-proxy gjennom samme tunnel"
    - case: "Tunnelstatus"
      ssh: "Terminalvindu"
      shprot: "Ikon i systemstatusfeltet og varsler med årsaken"
  note: "Verktøy som `autossh` hjelper med omstart, men krever fortsatt manuelt oppsett og har ikke noe grensesnitt."

uses:
  title: "Hva den passer til"
  items:
    - id: infra
      title: "Din egen infrastruktur"
      text: "Nå interne dashbord, testmiljøer og administrasjonsverktøy som bare er tilgjengelige fra serverens nettverk."
    - id: wifi
      title: "Usikre nettverk"
      text: "Send trafikken gjennom din egen server når du bruker offentlig wifi på kafeer, hoteller eller flyplasser."
    - id: laptop
      title: "Fjernarbeid på bærbar"
      text: "En stabil proxy på en datamaskin som går i dvale og bytter nettverk hele dagen."

how:
  title: "I gang i tre trinn"
  steps:
    - title: "Klargjør en server"
      text: "Enhver Linux-server med OpenSSH holder. Ingenting installeres på den — en egen bruker som bare får videresende porter, lages med noen få kommandoer."
    - title: "Koble til Shprot"
      text: "Skriv inn `user@server`, lag en nøkkel og legg den offentlige delen til i `authorized_keys` på serveren. Sjekk serverens fingeravtrykk én gang."
    - title: "Pek programmene mot proxyen"
      text: "Bruk `socks5h://localhost:1080` i nettleseren eller systemets proxyinnstillinger — menyen i systemstatusfeltet kopierer adressen for deg."
  guide: "Trinnvis veiledning med sikring av serveren"

tour:
  title: "Et nærmere blikk"
  items:
    - id: trust
      caption: "Ved første tilkobling ser du fingeravtrykket til serverens nøkkel og bestemmer om du stoler på den."
    - id: tray
      caption: "Menyen i systemstatusfeltet viser tunnelens status og kopierer ferdige proxyadresser."

download:
  title: "Last ned Shprot"
  lead: "Versjon {version}. Gratis, åpen kildekode, uten kontoer eller abonnementer."
  windows:
    title: "Windows"
    text: "Windows 10 og 11, 64-biters. Oppdater ved å installere den nye versjonen over den gamle."
    button: "Installasjonsprogram (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ og kompatible distribusjoner."
    button: "Pakke (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ og kompatible distribusjoner."
    button: "Pakke (.rpm)"
  requirements: "Shprot bruker OpenSSH-klienten (`ssh` og `ssh-keygen`). Den følger med Windows 10 / 11 og de fleste Linux-skrivebord; mangler den, forteller Shprot deg hvordan du legger den til."
  verify: "Hver versjon har en `SHA256SUMS`-fil, slik at du kan sjekke at nedlastingen ikke er endret."
  all_releases: "Alle versjoner og endringer"

faq:
  title: "Spørsmål og svar"
  items:
    - q: "Er Shprot en VPN?"
      a: "Nei, det er en proxy. Bare programmer som er satt opp til å bruke den — en nettleser, `curl`, `git`, en IDE — sender trafikk gjennom tunnelen; alt annet går direkte. Ingen virtuelle nettverkskort installeres."
    - q: "Hva trenger jeg på serveren?"
      a: "En vanlig OpenSSH-server og en konto som har lov til å videresende TCP-porter. Ingenting må installeres på serveren; enhver VPS eller hjemmeserver du når via SSH, fungerer."
    - q: "Kan jeg logge inn med passord?"
      a: "Nei, Shprot bruker bare SSH-nøkler. Nøkler er sikrere enn passord og lar tunnelen koble til på nytt uten at du gjør noe. Nøkkelen lages i appen med ett klikk."
    - q: "Hvilke programmer kan bruke proxyen?"
      a: "Nettlesere, `curl`, `git`, pakkebehandlere og alle programmer med proxyinnstillinger for SOCKS5 eller HTTP eller støtte for variablene `ALL_PROXY` / `HTTP_PROXY`. Bruk `socks5h://` slik at nettstedsnavn slås opp på serveren."
    - q: "Hvordan sjekker Shprot tunnelen?"
      a: "Av og til åpner den et tilfeldig valgt populært nettsted gjennom proxyen. Hvis det mislykkes mens internett er tilgjengelig, startes tunnelen på nytt."
    - q: "Er det gratis?"
      a: "Ja. Shprot er fri programvare med åpen kildekode under lisensen GNU GPL v3, uten kontoer, abonnementer eller reklame."
    - q: "Finnes det en macOS-versjon?"
      a: "Ikke ennå. Shprot støtter foreløpig Windows 10 / 11 og Linux."

footer:
  license: "Shprot er fri programvare under lisensen GNU GPL v3."
  source: "Kildekode"
  issues: "Rapporter et problem"
---
