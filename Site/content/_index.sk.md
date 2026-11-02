---
title: "Shprot — SOCKS5 proxy cez SSH, ktoré sa samo znova pripojí"
description: "Bezplatná aplikácia pre Windows a Linux, ktorá udržiava v chode SOCKS5 a HTTP proxy cez SSH: znova sa pripojí po zmene siete a spánku, odhalí zaseknuté tunely, bez príkazového riadka."
readme: "README.md"

ui:
  skip: "Prejsť na obsah"
  nav_features: "Funkcie"
  nav_compare: "Prečo Shprot"
  nav_download: "Stiahnuť"
  nav_faq: "Otázky"
  language: "Jazyk"
  github: "Zdrojový kód na GitHube"

hero:
  title: "SOCKS5 proxy cez SSH, ktoré zostáva pripojené"
  lead: "Shprot premení akýkoľvek server, ku ktorému sa dostanete cez SSH, na stabilné SOCKS5 a HTTP proxy. Stráži sieť, obnoví tunel po zmene Wi-Fi a prebudení zo spánku a presne vám povie, čo sa pokazilo — bez príkazového riadka."
  download: "Stiahnuť"
  download_hint: "Verzia {version} pre Windows a Linux"
  facts:
    - "Zadarmo a open source"
    - "Windows 10 / 11 a Linux"
    - "49 jazykov"
  screenshot_alt: "Okno nastavení aplikácie Shprot"

features:
  title: "Všetko, čo tunel potrebuje, aby vydržal"
  items:
    - id: reconnect
      title: "Prežije zmenu siete"
      text: "Prepnutie Wi-Fi, zapojený kábel alebo notebook prebudený zo spánku — Shprot to zistí a tunel sám obnoví."
    - id: health
      title: "Odhalí tiché výpadky"
      text: "Pravidelné kontroly cez samotné proxy nájdu tunel, ktorý vyzerá pripojený, no neprepúšťa prevádzku, a reštartujú ho."
    - id: gentle
      title: "Šetrný k vášmu serveru"
      text: "Ďalšie pokusy sa postupne zriedkavejú a po neúspešnom prihlásení sa opakujú najviac raz za minútu, takže vám fail2ban pre zlý kľúč nezablokuje IP adresu."
    - id: keys
      title: "SSH kľúče na dve kliknutia"
      text: "Vytvorte alebo importujte kľúč a skopírujte jeho verejnú časť do `authorized_keys` priamo z okna nastavení."
    - id: trust
      title: "Zrozumiteľné overenie servera"
      text: "Pri prvom pripojení sa odtlačok kľúča servera zobrazí v dialógu, aby ste ho mohli skontrolovať skôr, než serveru začnete dôverovať."
    - id: http
      title: "Aj HTTP proxy"
      text: "Aplikácie bez podpory SOCKS5 môžu používať voliteľné HTTP proxy, ktoré vedie tým istým tunelom."
    - id: tray
      title: "Býva v oblasti oznámení"
      text: "Ikona ukazuje, či tunel beží, oznámenia vysvetlia, prečo nie, a denník je na jedno kliknutie."
    - id: languages
      title: "Hovorí vaším jazykom"
      text: "Rozhranie je preložené do 49 jazykov, pre arabčinu, perzštinu, hebrejčinu a urdčinu s rozložením sprava doľava."

compare:
  title: "Prečo nie jednoducho `ssh -D`?"
  lead: "`ssh -D 1080 user@server` vám dá SOCKS5 proxy jedným príkazom — kým spojenie potichu neprestane fungovať. Shprot spúšťa toho istého klienta OpenSSH a postará sa o všetko, čo samotný príkaz nerobí."
  rows:
    - case: "Zmena siete alebo Wi-Fi, prebudenie zo spánku"
      ssh: "Reštartujte sami"
      shprot: "Znova sa pripojí automaticky"
    - case: "Tunel pripojený, ale bez prevádzky"
      ssh: "Zostane nepovšimnuté"
      shprot: "Odhalia ho kontroly cez proxy"
    - case: "Opakované chyby overenia"
      ssh: "Závisí od vašich skriptov"
      shprot: "Zriedkavé pokusy, aby server nezablokoval vašu IP"
    - case: "Kľúče klienta"
      ssh: "`ssh-keygen` a súbory v `~/.ssh`"
      shprot: "Vytvoriť alebo importovať, skopírovať verejný kľúč jedným kliknutím"
    - case: "Overenie identity servera"
      ssh: "Otázka v termináli"
      shprot: "Dialóg s odtlačkom kľúča servera"
    - case: "Aplikácie bez podpory SOCKS5"
      ssh: "Nepodporované"
      shprot: "Voliteľné HTTP proxy tým istým tunelom"
    - case: "Stav tunela"
      ssh: "Okno terminálu"
      shprot: "Ikona a oznámenia s príčinou chyby"
  note: "Nástroje ako `autossh` pomáhajú s reštartmi, ale stále vyžadujú ručné nastavenie a nemajú rozhranie."

uses:
  title: "Na čo sa hodí"
  items:
    - id: infra
      title: "Vlastná infraštruktúra"
      text: "Dostanete sa k interným prehľadom, testovacím prostrediam a administrátorským nástrojom dostupným len zo siete vášho servera."
    - id: wifi
      title: "Nedôveryhodné siete"
      text: "Posielajte prevádzku cez vlastný server, keď používate verejnú Wi-Fi v kaviarňach, hoteloch alebo na letiskách."
    - id: laptop
      title: "Práca na diaľku s notebookom"
      text: "Stabilné proxy na počítači, ktorý celý deň zaspáva a mení siete."

how:
  title: "Hotovo v troch krokoch"
  steps:
    - title: "Pripravte server"
      text: "Stačí akýkoľvek linuxový server s OpenSSH. Nič sa naň neinštaluje — samostatného používateľa, ktorý smie len presmerovávať porty, vytvoríte niekoľkými príkazmi."
    - title: "Pripojte Shprot"
      text: "Zadajte `user@server`, vytvorte kľúč a jeho verejnú časť pridajte do `authorized_keys` na serveri. Raz skontrolujte odtlačok servera."
    - title: "Nasmerujte aplikácie na proxy"
      text: "Použite `socks5h://localhost:1080` v prehliadači alebo v systémovom nastavení proxy — ponuka v oblasti oznámení adresu skopíruje za vás."
  guide: "Návod krok za krokom vrátane zabezpečenia servera"

tour:
  title: "Zblízka"
  items:
    - id: trust
      caption: "Pri prvom pripojení uvidíte odtlačok kľúča servera a rozhodnete, či mu dôverovať."
    - id: tray
      caption: "Ponuka v oblasti oznámení ukazuje stav tunela a kopíruje pripravené adresy proxy."

download:
  title: "Stiahnuť Shprot"
  lead: "Verzia {version}. Zadarmo, open source, bez účtov a predplatného."
  windows:
    title: "Windows"
    text: "Windows 10 a 11, 64-bitové. Na aktualizáciu nainštalujte novú verziu cez starú."
    button: "Inštalátor (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ a kompatibilné distribúcie."
    button: "Balík (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ a kompatibilné distribúcie."
    button: "Balík (.rpm)"
  requirements: "Shprot používa klienta OpenSSH (`ssh` a `ssh-keygen`). Je súčasťou Windows 10 / 11 aj väčšiny linuxových prostredí; ak chýba, Shprot vám poradí, ako ho doplniť."
  verify: "Každé vydanie obsahuje súbor `SHA256SUMS`, ktorým overíte, že stiahnutý súbor nebol zmenený."
  all_releases: "Všetky vydania a zmeny"

faq:
  title: "Otázky a odpovede"
  items:
    - q: "Je Shprot VPN?"
      a: "Nie, je to proxy. Tunelom posielajú prevádzku len aplikácie, ktoré sú na to nastavené — prehliadač, `curl`, `git`, IDE; všetko ostatné ide priamo. Neinštalujú sa žiadne virtuálne sieťové adaptéry."
    - q: "Čo potrebujem na serveri?"
      a: "Bežný server OpenSSH a účet, ktorý smie presmerovávať TCP porty. Na server sa nič neinštaluje; poslúži akýkoľvek VPS alebo domáci server dostupný cez SSH."
    - q: "Môžem sa prihlasovať heslom?"
      a: "Nie, Shprot používa iba SSH kľúče. Kľúče sú bezpečnejšie ako heslá a umožňujú tunelu znova sa pripojiť bez vášho zásahu. Kľúč vytvoríte v aplikácii jedným kliknutím."
    - q: "Ktoré aplikácie môžu proxy používať?"
      a: "Prehliadače, `curl`, `git`, správcovia balíkov a akékoľvek aplikácie s nastavením SOCKS5 alebo HTTP proxy či s podporou premenných `ALL_PROXY` / `HTTP_PROXY`. Používajte `socks5h://`, aby sa názvy webov prekladali na serveri."
    - q: "Ako Shprot kontroluje tunel?"
      a: "Z času na čas cez proxy otvorí náhodne vybraný populárny web. Ak to zlyhá, hoci je internet dostupný, tunel sa reštartuje."
    - q: "Je to zadarmo?"
      a: "Áno. Shprot je slobodný softvér s otvoreným zdrojovým kódom pod licenciou GNU GPL v3, bez účtov, predplatného a reklám."
    - q: "Existuje verzia pre macOS?"
      a: "Zatiaľ nie. Shprot teraz podporuje Windows 10 / 11 a Linux."

footer:
  license: "Shprot je slobodný softvér pod licenciou GNU GPL v3."
  source: "Zdrojový kód"
  issues: "Nahlásiť problém"
---
