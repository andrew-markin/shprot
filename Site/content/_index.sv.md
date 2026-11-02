---
title: "Shprot — SOCKS5-proxy över SSH som återansluter av sig själv"
description: "Gratis skrivbordsapp för Windows och Linux som håller en SOCKS5- och HTTP-proxy över SSH igång: återansluter efter nätverksbyten och viloläge, upptäcker tunnlar som hängt sig, utan kommandorad."
readme: "README.md"

ui:
  skip: "Hoppa till innehållet"
  nav_features: "Funktioner"
  nav_compare: "Varför Shprot"
  nav_download: "Ladda ner"
  nav_faq: "Frågor"
  language: "Språk"
  github: "Källkod på GitHub"

hero:
  title: "En SOCKS5-proxy över SSH som håller anslutningen"
  lead: "Shprot gör vilken server du än når via SSH till en stabil SOCKS5- och HTTP-proxy. Appen bevakar nätverket, återställer tunneln efter wifi-byte och viloläge och berättar exakt vad som gick fel — helt utan kommandorad."
  download: "Ladda ner"
  download_hint: "Version {version} för Windows och Linux"
  facts:
    - "Gratis och öppen källkod"
    - "Windows 10 / 11 och Linux"
    - "49 språk"
  screenshot_alt: "Shprots inställningsfönster"

features:
  title: "Allt en tunnel behöver för att fortsätta fungera"
  items:
    - id: reconnect
      title: "Klarar nätverksbyten"
      text: "Byte av wifi, en ansluten kabel eller en bärbar som vaknar ur viloläge — Shprot märker det och återställer tunneln på egen hand."
    - id: health
      title: "Fångar tysta fel"
      text: "Regelbundna kontroller genom själva proxyn hittar en tunnel som ser ansluten ut men inte släpper igenom trafik, och startar om den."
    - id: gentle
      title: "Skonsam mot din server"
      text: "Nya försök glesas ut gradvis och efter misslyckade inloggningar görs de högst en gång i minuten, så fail2ban spärrar inte din IP på grund av en felaktig nyckel."
    - id: keys
      title: "SSH-nycklar med två klick"
      text: "Skapa eller importera en nyckel och kopiera den publika delen till `authorized_keys` direkt från inställningsfönstret."
    - id: trust
      title: "Tydlig serverkontroll"
      text: "Vid första anslutningen visas fingeravtrycket för serverns värdnyckel i en dialog, så att du kan kontrollera det innan du litar på servern."
    - id: http
      title: "HTTP-proxy också"
      text: "Program utan stöd för SOCKS5 kan använda en valfri HTTP-proxy som går genom samma tunnel."
    - id: tray
      title: "Bor i systemfältet"
      text: "Ikonen visar om tunneln är uppe, aviseringar förklarar varför inte, och loggarna finns ett klick bort."
    - id: languages
      title: "Talar ditt språk"
      text: "Gränssnittet är översatt till 49 språk, med layout från höger till vänster för arabiska, persiska, hebreiska och urdu."

compare:
  title: "Varför inte bara `ssh -D`?"
  lead: "`ssh -D 1080 user@server` ger dig en SOCKS5-proxy med ett kommando — tills anslutningen tyst slutar fungera. Shprot kör samma OpenSSH-klient och sköter allt som kommandot inte gör på egen hand."
  rows:
    - case: "Nätverksbyte, wifi-byte, uppvaknande ur viloläge"
      ssh: "Starta om själv"
      shprot: "Återansluter automatiskt"
    - case: "Tunneln ansluten men utan trafik"
      ssh: "Går obemärkt förbi"
      shprot: "Upptäcks av kontroller genom proxyn"
    - case: "Upprepade autentiseringsfel"
      ssh: "Upp till dina skript"
      shprot: "Sällsynta försök, så att servern inte spärrar din IP"
    - case: "Klientnycklar"
      ssh: "`ssh-keygen` och filer i `~/.ssh`"
      shprot: "Skapa eller importera, kopiera den publika nyckeln med ett klick"
    - case: "Kontroll av serverns identitet"
      ssh: "Fråga i terminalen"
      shprot: "Dialog med värdnyckelns fingeravtryck"
    - case: "Program utan stöd för SOCKS5"
      ssh: "Inte täckt"
      shprot: "Valfri HTTP-proxy genom samma tunnel"
    - case: "Tunnelstatus"
      ssh: "Terminalfönster"
      shprot: "Ikon i systemfältet och aviseringar med orsaken"
  note: "Verktyg som `autossh` hjälper med omstarter, men kräver ändå manuell konfiguration och saknar gränssnitt."

uses:
  title: "Vad den passar till"
  items:
    - id: infra
      title: "Din egen infrastruktur"
      text: "Nå interna instrumentpaneler, testmiljöer och adminverktyg som bara är åtkomliga från serverns nätverk."
    - id: wifi
      title: "Osäkra nätverk"
      text: "Skicka din trafik via din egen server när du använder offentligt wifi på kaféer, hotell eller flygplatser."
    - id: laptop
      title: "Distansarbete på bärbar dator"
      text: "En stabil proxy på en dator som går i viloläge och byter nätverk hela dagen."

how:
  title: "Igång i tre steg"
  steps:
    - title: "Förbered en server"
      text: "Vilken Linux-server som helst med OpenSSH duger. Inget installeras på den — en egen användare som bara får vidarebefordra portar skapas med några kommandon."
    - title: "Anslut Shprot"
      text: "Ange `user@server`, skapa en nyckel och lägg till den publika delen i `authorized_keys` på servern. Kontrollera serverns fingeravtryck en gång."
    - title: "Peka programmen mot proxyn"
      text: "Använd `socks5h://localhost:1080` i webbläsaren eller systemets proxyinställningar — menyn i systemfältet kopierar adressen åt dig."
  guide: "Steg-för-steg-guide med härdning av servern"

tour:
  title: "En närmare titt"
  items:
    - id: trust
      caption: "Vid första anslutningen ser du fingeravtrycket för serverns nyckel och bestämmer om du litar på den."
    - id: tray
      caption: "Menyn i systemfältet visar tunnelns status och kopierar färdiga proxyadresser."

download:
  title: "Ladda ner Shprot"
  lead: "Version {version}. Gratis, öppen källkod, utan konton eller prenumerationer."
  windows:
    title: "Windows"
    text: "Windows 10 och 11, 64-bitars. Uppdatera genom att installera den nya versionen ovanpå den gamla."
    button: "Installationsprogram (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ och kompatibla distributioner."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ och kompatibla distributioner."
    button: "Paket (.rpm)"
  requirements: "Shprot använder OpenSSH-klienten (`ssh` och `ssh-keygen`). Den ingår i Windows 10 / 11 och de flesta Linux-skrivbord; saknas den berättar Shprot hur du lägger till den."
  verify: "Varje version innehåller en `SHA256SUMS`-fil för att kontrollera att nedladdningen inte har ändrats."
  all_releases: "Alla versioner och ändringar"

faq:
  title: "Frågor och svar"
  items:
    - q: "Är Shprot en VPN?"
      a: "Nej, det är en proxy. Bara program som är inställda att använda den — en webbläsare, `curl`, `git`, en IDE — skickar trafik genom tunneln; allt annat går direkt. Inga virtuella nätverkskort installeras."
    - q: "Vad behöver jag på servern?"
      a: "En vanlig OpenSSH-server och ett konto som får vidarebefordra TCP-portar. Inget behöver installeras på servern; vilken VPS eller hemserver som helst som nås via SSH fungerar."
    - q: "Kan jag logga in med lösenord?"
      a: "Nej, Shprot använder bara SSH-nycklar. Nycklar är säkrare än lösenord och låter tunneln återansluta utan att du behöver göra något. Nyckeln skapas i appen med ett klick."
    - q: "Vilka program kan använda proxyn?"
      a: "Webbläsare, `curl`, `git`, pakethanterare och alla program med proxyinställningar för SOCKS5 eller HTTP eller stöd för variablerna `ALL_PROXY` / `HTTP_PROXY`. Använd `socks5h://` så att webbplatsnamn slås upp på servern."
    - q: "Hur kontrollerar Shprot tunneln?"
      a: "Då och då öppnar den en slumpvis vald populär webbplats genom proxyn. Misslyckas det trots att internet är tillgängligt startas tunneln om."
    - q: "Är det gratis?"
      a: "Ja. Shprot är fri programvara med öppen källkod under licensen GNU GPL v3, utan konton, prenumerationer eller reklam."
    - q: "Finns det en version för macOS?"
      a: "Inte än. Shprot stöder för närvarande Windows 10 / 11 och Linux."

footer:
  license: "Shprot är fri programvara under licensen GNU GPL v3."
  source: "Källkod"
  issues: "Rapportera ett problem"
---
