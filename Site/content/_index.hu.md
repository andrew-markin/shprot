---
title: "Shprot — SSH-n keresztüli SOCKS5 proxy, amely magától újracsatlakozik"
description: "Ingyenes asztali alkalmazás Windowsra és Linuxra, amely működésben tartja az SSH-n keresztüli SOCKS5 és HTTP proxyt: hálózatváltás és alvó állapot után újracsatlakozik, észreveszi az elakadt alagutakat, parancssor nélkül."
readme: "README.md"

ui:
  skip: "Ugrás a tartalomhoz"
  nav_features: "Funkciók"
  nav_compare: "Miért a Shprot"
  nav_download: "Letöltés"
  nav_faq: "GYIK"
  language: "Nyelv"
  github: "Forráskód a GitHubon"

hero:
  title: "SSH-n keresztüli SOCKS5 proxy, amely kapcsolatban marad"
  lead: "A Shprot bármely SSH-n elérhető szerverből stabil SOCKS5 és HTTP proxyt csinál. Figyeli a hálózatot, Wi-Fi-váltás és alvó állapot után helyreállítja az alagutat, és pontosan megmondja, mi romlott el — parancssor nélkül."
  download: "Letöltés"
  download_hint: "{version} verzió Windowsra és Linuxra"
  facts:
    - "Ingyenes és nyílt forráskódú"
    - "Windows 10 / 11 és Linux"
    - "49 nyelv"
  screenshot_alt: "A Shprot beállítási ablaka"

features:
  title: "Minden, ami ahhoz kell, hogy az alagút működjön"
  items:
    - id: reconnect
      title: "Túléli a hálózatváltást"
      text: "Wi-Fi-váltás, bedugott kábel vagy alvó állapotból felébresztett laptop — a Shprot észreveszi, és magától helyreállítja az alagutat."
    - id: health
      title: "Elkapja a csendes hibákat"
      text: "A magán a proxyn keresztül végzett rendszeres ellenőrzések megtalálják azt az alagutat, amely csatlakozottnak látszik, de nem enged át forgalmat, és újraindítják."
    - id: gentle
      title: "Kíméli a szervert"
      text: "Az újrapróbálkozások fokozatosan ritkulnak, sikertelen bejelentkezés után legfeljebb percenként egyszer ismétlődnek, így a fail2ban nem tiltja le az IP-címét egy rossz kulcs miatt."
    - id: keys
      title: "SSH-kulcsok két kattintással"
      text: "Hozzon létre vagy importáljon kulcsot, és másolja a nyilvános részét az `authorized_keys` fájlhoz közvetlenül a beállítási ablakból."
    - id: trust
      title: "Átlátható szerverellenőrzés"
      text: "Az első csatlakozáskor a szerver gazdakulcsának ujjlenyomata egy párbeszédablakban jelenik meg, így ellenőrizheti, mielőtt megbízna a szerverben."
    - id: http
      title: "HTTP proxy is"
      text: "A SOCKS5-öt nem támogató alkalmazások egy választható HTTP proxyt használhatnak, amely ugyanazon az alagúton halad át."
    - id: tray
      title: "A tálcán lakik"
      text: "Az ikon mutatja, hogy nyitva van-e az alagút, az értesítések elmagyarázzák, miért nincs, a napló pedig egy kattintásnyira van."
    - id: languages
      title: "Beszéli a nyelvét"
      text: "A felület 49 nyelvre van lefordítva, az arab, perzsa, héber és urdu nyelvhez jobbról balra haladó elrendezéssel."

compare:
  title: "Miért nem elég az `ssh -D`?"
  lead: "Az `ssh -D 1080 user@server` egyetlen paranccsal ad SOCKS5 proxyt — egészen addig, amíg a kapcsolat csendben le nem áll. A Shprot ugyanazt az OpenSSH klienst futtatja, és elintéz mindent, amit a parancs önmagában nem."
  rows:
    - case: "Hálózat- vagy Wi-Fi-váltás, ébredés alvó állapotból"
      ssh: "Indítsa újra kézzel"
      shprot: "Automatikusan újracsatlakozik"
    - case: "Az alagút csatlakozott, de nincs forgalom"
      ssh: "Észrevétlen marad"
      shprot: "A proxyn keresztüli ellenőrzések kiszúrják"
    - case: "Ismétlődő hitelesítési hibák"
      ssh: "A szkriptjeire van bízva"
      shprot: "Ritka próbálkozások, hogy a szerver ne tiltsa le az IP-címét"
    - case: "Kliens kulcsok"
      ssh: "`ssh-keygen` és fájlok a `~/.ssh` mappában"
      shprot: "Létrehozás vagy importálás, a nyilvános kulcs másolása egy kattintással"
    - case: "A szerver azonosságának ellenőrzése"
      ssh: "Kérdés a terminálban"
      shprot: "Párbeszédablak a gazdakulcs ujjlenyomatával"
    - case: "SOCKS5-öt nem támogató alkalmazások"
      ssh: "Nincs megoldás"
      shprot: "Választható HTTP proxy ugyanazon az alagúton"
    - case: "Az alagút állapota"
      ssh: "Terminálablak"
      shprot: "Tálcaikon és értesítések a hiba okával"
  note: "Az olyan eszközök, mint az `autossh`, segítenek az újraindításban, de továbbra is kézi beállítást igényelnek, és nincs felületük."

uses:
  title: "Mire jó"
  items:
    - id: infra
      title: "Saját infrastruktúra"
      text: "Érje el a belső irányítópultokat, tesztkörnyezeteket és adminisztrációs eszközöket, amelyek csak a szerver hálózatából érhetők el."
    - id: wifi
      title: "Megbízhatatlan hálózatok"
      text: "Küldje a forgalmát a saját szerverén keresztül, amikor nyilvános Wi-Fit használ kávézókban, szállodákban vagy repülőtereken."
    - id: laptop
      title: "Távmunka laptopon"
      text: "Stabil proxy egy gépen, amely egész nap alvó állapotba kerül és hálózatot vált."

how:
  title: "Három lépésben kész"
  steps:
    - title: "Készítsen elő egy szervert"
      text: "Bármilyen Linux szerver megfelel OpenSSH-val. Semmit nem kell rá telepíteni — egy csak porttovábbításra jogosult külön felhasználó néhány paranccsal létrehozható."
    - title: "Csatlakoztassa a Shprotot"
      text: "Adja meg a `user@server` címet, hozzon létre kulcsot, és adja hozzá a nyilvános részét a szerver `authorized_keys` fájljához. Egyszer ellenőrizze a szerver ujjlenyomatát."
    - title: "Irányítsa az alkalmazásokat a proxyra"
      text: "Használja a `socks5h://localhost:1080` címet a böngészőben vagy a rendszer proxybeállításaiban — a tálcamenü kimásolja Önnek a címet."
  guide: "Lépésenkénti útmutató a szerver védelmével"

tour:
  title: "Közelebbről"
  items:
    - id: trust
      caption: "Az első csatlakozáskor látja a szerver kulcsának ujjlenyomatát, és eldönti, megbízik-e benne."
    - id: tray
      caption: "A tálcamenü mutatja az alagút állapotát, és használatra kész proxycímeket másol."

download:
  title: "A Shprot letöltése"
  lead: "{version} verzió. Ingyenes, nyílt forráskódú, fiók és előfizetés nélkül."
  windows:
    title: "Windows"
    text: "Windows 10 és 11, 64 bites. Frissítéshez telepítse az új verziót a régire."
    button: "Telepítő (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ és kompatibilis disztribúciók."
    button: "Csomag (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ és kompatibilis disztribúciók."
    button: "Csomag (.rpm)"
  requirements: "A Shprot az OpenSSH klienst használja (`ssh` és `ssh-keygen`). Ez része a Windows 10 / 11-nek és a legtöbb Linux asztali környezetnek; ha hiányzik, a Shprot elmondja, hogyan pótolhatja."
  verify: "Minden kiadás tartalmaz egy `SHA256SUMS` fájlt, amellyel ellenőrizheti, hogy a letöltött fájlt nem módosították."
  all_releases: "Az összes kiadás és változás"

faq:
  title: "Kérdések és válaszok"
  items:
    - q: "A Shprot VPN?"
      a: "Nem, ez egy proxy. Csak az erre beállított alkalmazások — böngésző, `curl`, `git`, IDE — küldenek forgalmat az alagúton; minden más közvetlenül megy. Virtuális hálózati adapter nem települ."
    - q: "Mi kell a szerveren?"
      a: "Egy szokásos OpenSSH szerver és egy fiók, amely TCP-portokat továbbíthat. A szerverre semmit nem kell telepíteni; bármilyen SSH-n elérhető VPS vagy otthoni szerver megfelel."
    - q: "Bejelentkezhetek jelszóval?"
      a: "Nem, a Shprot csak SSH-kulcsokat használ. A kulcsok biztonságosabbak a jelszavaknál, és lehetővé teszik, hogy az alagút beavatkozás nélkül újracsatlakozzon. A kulcs az alkalmazásban egy kattintással létrehozható."
    - q: "Mely alkalmazások használhatják a proxyt?"
      a: "Böngészők, `curl`, `git`, csomagkezelők és bármely alkalmazás, amely rendelkezik SOCKS5 vagy HTTP proxybeállításokkal, vagy támogatja az `ALL_PROXY` / `HTTP_PROXY` változókat. Használja a `socks5h://` címet, hogy a webhelyek nevét a szerver oldja fel."
    - q: "Hogyan ellenőrzi a Shprot az alagutat?"
      a: "Időnként megnyit a proxyn keresztül egy véletlenszerűen választott népszerű webhelyet. Ha ez nem sikerül, miközben az internet elérhető, az alagút újraindul."
    - q: "Ingyenes?"
      a: "Igen. A Shprot szabad, nyílt forráskódú szoftver GNU GPL v3 licenccel, fiókok, előfizetések és hirdetések nélkül."
    - q: "Van macOS-verzió?"
      a: "Még nincs. A Shprot jelenleg Windows 10 / 11 és Linux rendszeren működik."

footer:
  license: "A Shprot szabad szoftver GNU GPL v3 licenccel."
  source: "Forráskód"
  issues: "Hiba bejelentése"
---
