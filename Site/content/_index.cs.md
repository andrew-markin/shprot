---
title: "Shprot — SOCKS5 proxy přes SSH, které se samo znovu připojí"
description: "Bezplatná aplikace pro Windows a Linux, která udržuje v chodu SOCKS5 a HTTP proxy přes SSH: znovu se připojí po změně sítě a spánku, odhalí zaseknuté tunely, bez příkazové řádky."
readme: "README.md"

ui:
  skip: "Přejít k obsahu"
  nav_features: "Funkce"
  nav_compare: "Proč Shprot"
  nav_download: "Stáhnout"
  nav_faq: "Otázky"
  language: "Jazyk"
  github: "Zdrojový kód na GitHubu"

hero:
  title: "SOCKS5 proxy přes SSH, které zůstává připojené"
  lead: "Shprot promění jakýkoli server, na který se dostanete přes SSH, ve stabilní SOCKS5 a HTTP proxy. Hlídá síť, obnoví tunel po změně Wi-Fi a probuzení ze spánku a přesně vám řekne, co se pokazilo — bez příkazové řádky."
  download: "Stáhnout"
  download_hint: "Verze {version} pro Windows a Linux"
  facts:
    - "Zdarma a open source"
    - "Windows 10 / 11 a Linux"
    - "49 jazyků"
  screenshot_alt: "Okno nastavení aplikace Shprot"

features:
  title: "Vše, co tunel potřebuje, aby vydržel"
  items:
    - id: reconnect
      title: "Přežije změnu sítě"
      text: "Přepnutí Wi-Fi, zapojený kabel nebo notebook probuzený ze spánku — Shprot to pozná a tunel sám obnoví."
    - id: health
      title: "Odhalí tiché výpadky"
      text: "Pravidelné kontroly přes samotné proxy najdou tunel, který vypadá připojeně, ale nepropouští provoz, a restartují ho."
    - id: gentle
      title: "Šetrný k vašemu serveru"
      text: "Další pokusy se postupně zřeďují a po neúspěšném přihlášení se opakují nejvýše jednou za minutu, takže vám fail2ban kvůli špatnému klíči nezablokuje IP adresu."
    - id: keys
      title: "SSH klíče na dvě kliknutí"
      text: "Vytvořte nebo importujte klíč a zkopírujte jeho veřejnou část do `authorized_keys` přímo z okna nastavení."
    - id: trust
      title: "Srozumitelné ověření serveru"
      text: "Při prvním připojení se otisk klíče serveru zobrazí v dialogu, abyste jej mohli zkontrolovat dřív, než serveru začnete důvěřovat."
    - id: http
      title: "I HTTP proxy"
      text: "Aplikace bez podpory SOCKS5 mohou používat volitelné HTTP proxy, které vede stejným tunelem."
    - id: tray
      title: "Bydlí v oznamovací oblasti"
      text: "Ikona ukazuje, zda tunel běží, oznámení vysvětlí, proč ne, a protokol je na jedno kliknutí."
    - id: languages
      title: "Mluví vaším jazykem"
      text: "Rozhraní je přeloženo do 49 jazyků, pro arabštinu, perštinu, hebrejštinu a urdštinu s rozložením zprava doleva."

compare:
  title: "Proč ne prostě `ssh -D`?"
  lead: "`ssh -D 1080 user@server` vám dá SOCKS5 proxy jedním příkazem — dokud spojení potichu nepřestane fungovat. Shprot spouští stejného klienta OpenSSH a postará se o vše, co samotný příkaz nedělá."
  rows:
    - case: "Změna sítě nebo Wi-Fi, probuzení ze spánku"
      ssh: "Restartujte sami"
      shprot: "Znovu se připojí automaticky"
    - case: "Tunel připojený, ale bez provozu"
      ssh: "Zůstane bez povšimnutí"
      shprot: "Odhalí ho kontroly přes proxy"
    - case: "Opakované chyby ověření"
      ssh: "Záleží na vašich skriptech"
      shprot: "Řídké pokusy, aby server nezablokoval vaši IP"
    - case: "Klíče klienta"
      ssh: "`ssh-keygen` a soubory v `~/.ssh`"
      shprot: "Vytvořit nebo importovat, zkopírovat veřejný klíč jedním kliknutím"
    - case: "Ověření identity serveru"
      ssh: "Dotaz v terminálu"
      shprot: "Dialog s otiskem klíče serveru"
    - case: "Aplikace bez podpory SOCKS5"
      ssh: "Nepodporováno"
      shprot: "Volitelné HTTP proxy stejným tunelem"
    - case: "Stav tunelu"
      ssh: "Okno terminálu"
      shprot: "Ikona a oznámení s příčinou chyby"
  note: "Nástroje jako `autossh` pomáhají s restarty, ale stále vyžadují ruční nastavení a nemají rozhraní."

uses:
  title: "K čemu se hodí"
  items:
    - id: infra
      title: "Vlastní infrastruktura"
      text: "Dostanete se k interním přehledům, testovacím prostředím a administrátorským nástrojům dostupným jen ze sítě vašeho serveru."
    - id: wifi
      title: "Nedůvěryhodné sítě"
      text: "Posílejte provoz přes vlastní server, když používáte veřejnou Wi-Fi v kavárnách, hotelích nebo na letištích."
    - id: laptop
      title: "Práce na dálku s notebookem"
      text: "Stabilní proxy na počítači, který celý den usíná a mění sítě."

how:
  title: "Hotovo ve třech krocích"
  steps:
    - title: "Připravte server"
      text: "Stačí jakýkoli linuxový server s OpenSSH. Nic se na něj neinstaluje — samostatného uživatele, který smí jen přesměrovávat porty, vytvoříte několika příkazy."
    - title: "Připojte Shprot"
      text: "Zadejte `user@server`, vytvořte klíč a jeho veřejnou část přidejte do `authorized_keys` na serveru. Jednou zkontrolujte otisk serveru."
    - title: "Nasměrujte aplikace na proxy"
      text: "Použijte `socks5h://localhost:1080` v prohlížeči nebo v systémovém nastavení proxy — nabídka v oznamovací oblasti adresu zkopíruje za vás."
  guide: "Návod krok za krokem včetně zabezpečení serveru"

tour:
  title: "Zblízka"
  items:
    - id: trust
      caption: "Při prvním připojení uvidíte otisk klíče serveru a rozhodnete, zda mu důvěřovat."
    - id: tray
      caption: "Nabídka v oznamovací oblasti ukazuje stav tunelu a kopíruje připravené adresy proxy."

download:
  title: "Stáhnout Shprot"
  lead: "Verze {version}. Zdarma, open source, bez účtů a předplatného."
  windows:
    title: "Windows"
    text: "Windows 10 a 11, 64bitové. Pro aktualizaci nainstalujte novou verzi přes starou."
    button: "Instalátor (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ a kompatibilní distribuce."
    button: "Balíček (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ a kompatibilní distribuce."
    button: "Balíček (.rpm)"
  requirements: "Shprot používá klienta OpenSSH (`ssh` a `ssh-keygen`). Je součástí Windows 10 / 11 i většiny linuxových prostředí; pokud chybí, Shprot vám poradí, jak ho doplnit."
  verify: "Každé vydání obsahuje soubor `SHA256SUMS`, kterým ověříte, že stažený soubor nebyl změněn."
  all_releases: "Všechna vydání a změny"

faq:
  title: "Otázky a odpovědi"
  items:
    - q: "Je Shprot VPN?"
      a: "Ne, je to proxy. Tunelem posílají provoz jen aplikace, které jsou na to nastavené — prohlížeč, `curl`, `git`, IDE; vše ostatní jde přímo. Neinstalují se žádné virtuální síťové adaptéry."
    - q: "Co potřebuji na serveru?"
      a: "Běžný server OpenSSH a účet, který smí přesměrovávat TCP porty. Na server se nic neinstaluje; poslouží jakýkoli VPS nebo domácí server dostupný přes SSH."
    - q: "Můžu se přihlašovat heslem?"
      a: "Ne, Shprot používá pouze SSH klíče. Klíče jsou bezpečnější než hesla a umožňují tunelu znovu se připojit bez vašeho zásahu. Klíč vytvoříte v aplikaci jedním kliknutím."
    - q: "Které aplikace mohou proxy používat?"
      a: "Prohlížeče, `curl`, `git`, správci balíčků a jakékoli aplikace s nastavením SOCKS5 nebo HTTP proxy či s podporou proměnných `ALL_PROXY` / `HTTP_PROXY`. Používejte `socks5h://`, aby se názvy webů překládaly na serveru."
    - q: "Jak Shprot tunel kontroluje?"
      a: "Čas od času přes proxy otevře náhodně vybraný populární web. Pokud to selže, přestože je internet dostupný, tunel se restartuje."
    - q: "Je to zdarma?"
      a: "Ano. Shprot je svobodný software s otevřeným zdrojovým kódem pod licencí GNU GPL v3, bez účtů, předplatného a reklam."
    - q: "Existuje verze pro macOS?"
      a: "Zatím ne. Shprot nyní podporuje Windows 10 / 11 a Linux."

footer:
  license: "Shprot je svobodný software pod licencí GNU GPL v3."
  source: "Zdrojový kód"
  issues: "Nahlásit problém"
---
