---
title: "Shprot — proxy SOCKS5 përmes SSH që rilidhet vetë"
description: "Aplikacion falas për Windows dhe Linux që mban në punë një proxy SOCKS5 dhe HTTP përmes SSH: rilidhet pas ndryshimit të rrjetit dhe gjumit, zbulon tunelet e bllokuara, pa linjë komandash."
readme: "README.md"

ui:
  skip: "Kalo te përmbajtja"
  nav_features: "Veçoritë"
  nav_compare: "Pse Shprot"
  nav_download: "Shkarko"
  nav_faq: "Pyetje"
  language: "Gjuha"
  github: "Kodi burimor në GitHub"

hero:
  title: "Një proxy SOCKS5 përmes SSH që qëndron i lidhur"
  lead: "Shprot e kthen çdo server që e arrini me SSH në një proxy SOCKS5 dhe HTTP të qëndrueshëm. Ndjek rrjetin, e rikthen tunelin pas ndryshimit të Wi-Fi-së ose zgjimit nga gjumi dhe ju tregon saktësisht çfarë shkoi keq — pa linjë komandash."
  download: "Shkarko"
  download_hint: "Versioni {version} për Windows dhe Linux"
  facts:
    - "Falas dhe me burim të hapur"
    - "Windows 10 / 11 dhe Linux"
    - "49 gjuhë"
  screenshot_alt: "Dritarja e cilësimeve të Shprot"

features:
  title: "Gjithçka që i duhet një tuneli për të vazhduar punën"
  items:
    - id: reconnect
      title: "I mbijeton ndryshimit të rrjetit"
      text: "Ndryshoni Wi-Fi-në, lidhni një kabllo ose zgjoni laptopin — Shprot e vëren dhe e rikthen tunelin vetë."
    - id: health
      title: "Kap defektet e heshtura"
      text: "Kontrollet e rregullta përmes vetë proxy-t gjejnë një tunel që duket i lidhur, por nuk kalon trafik, dhe e rinisin."
    - id: gentle
      title: "I butë me serverin tuaj"
      text: "Riprovat rrallohen gradualisht dhe, pas hyrjeve të dështuara, përsëriten jo më shumë se një herë në minutë, kështu që fail2ban nuk do ta bllokojë IP-në tuaj për shkak të një çelësi të gabuar."
    - id: keys
      title: "Çelësa SSH me dy klikime"
      text: "Krijoni ose importoni një çelës dhe kopjoni pjesën publike për `authorized_keys` drejtpërdrejt nga dritarja e cilësimeve."
    - id: trust
      title: "Verifikim i qartë i serverit"
      text: "Në lidhjen e parë, gjurma e çelësit të serverit shfaqet në një dialog, që ta kontrolloni para se t’i besoni serverit."
    - id: http
      title: "Edhe proxy HTTP"
      text: "Aplikacionet pa mbështetje për SOCKS5 mund të përdorin një proxy HTTP opsional që kalon përmes të njëjtit tunel."
    - id: tray
      title: "Rri në hapësirën e njoftimeve"
      text: "Ikona tregon nëse tuneli është i hapur, njoftimet shpjegojnë pse jo, dhe regjistrat janë një klikim larg."
    - id: languages
      title: "Flet gjuhën tuaj"
      text: "Ndërfaqja është përkthyer në 49 gjuhë, me paraqitje nga e djathta në të majtë për arabishten, persishten, hebraishten dhe urdishten."

compare:
  title: "Pse jo thjesht `ssh -D`?"
  lead: "`ssh -D 1080 user@server` ju jep një proxy SOCKS5 me një komandë — derisa lidhja ndalon së punuari pa u vënë re. Shprot ekzekuton të njëjtin klient OpenSSH dhe kujdeset për gjithçka që komanda vetëm nuk e bën."
  rows:
    - case: "Ndryshim rrjeti ose Wi-Fi-je, zgjim nga gjumi"
      ssh: "Riniseni vetë"
      shprot: "Rilidhet automatikisht"
    - case: "Tuneli i lidhur, por pa trafik"
      ssh: "Kalon pa u vënë re"
      shprot: "Zbulohet nga kontrollet përmes proxy-t"
    - case: "Gabime të përsëritura vërtetimi"
      ssh: "Varet nga skriptet tuaja"
      shprot: "Riprova të rralla, që serveri të mos bllokojë IP-në tuaj"
    - case: "Çelësat e klientit"
      ssh: "`ssh-keygen` dhe skedarë në `~/.ssh`"
      shprot: "Krijo ose importo, kopjo çelësin publik me një klikim"
    - case: "Verifikimi i identitetit të serverit"
      ssh: "Pyetje në terminal"
      shprot: "Dialog me gjurmën e çelësit të serverit"
    - case: "Aplikacione pa mbështetje për SOCKS5"
      ssh: "Nuk mbulohen"
      shprot: "Proxy HTTP opsional përmes të njëjtit tunel"
    - case: "Gjendja e tunelit"
      ssh: "Dritare terminali"
      shprot: "Ikonë dhe njoftime me arsyen e dështimit"
  note: "Mjete si `autossh` ndihmojnë me rinisjet, por prapë kërkojnë konfigurim manual dhe nuk kanë ndërfaqe."

uses:
  title: "Për çfarë shërben"
  items:
    - id: infra
      title: "Infrastruktura juaj"
      text: "Arrini panele të brendshme, mjedise testimi dhe mjete administrimi që janë të arritshme vetëm nga rrjeti i serverit tuaj."
    - id: wifi
      title: "Rrjete të pasigurta"
      text: "Dërgojeni trafikun përmes serverit tuaj kur përdorni Wi-Fi publik në kafene, hotele ose aeroporte."
    - id: laptop
      title: "Punë në distancë me laptop"
      text: "Një proxy i qëndrueshëm në një kompjuter që fle dhe ndryshon rrjet gjithë ditën."

how:
  title: "Gati në tre hapa"
  steps:
    - title: "Përgatitni një server"
      text: "Shërben çdo server Linux me OpenSSH. Nuk instalohet asgjë — një përdorues i veçantë që mund vetëm të përcjellë porta krijohet me pak komanda."
    - title: "Lidhni Shprot"
      text: "Shkruani `user@server`, krijoni një çelës dhe shtoni pjesën publike në `authorized_keys` në server. Kontrolloni një herë gjurmën e serverit."
    - title: "Drejtojini aplikacionet te proxy"
      text: "Përdorni `socks5h://localhost:1080` në shfletues ose në cilësimet e proxy-t të sistemit — menyja e ikonës e kopjon adresën për ju."
  guide: "Udhëzues hap pas hapi me mbrojtjen e serverit"

tour:
  title: "Më nga afër"
  items:
    - id: trust
      caption: "Në lidhjen e parë shihni gjurmën e çelësit të serverit dhe vendosni nëse i besoni."
    - id: tray
      caption: "Menyja e ikonës tregon gjendjen e tunelit dhe kopjon adresa proxy gati për përdorim."

download:
  title: "Shkarkoni Shprot"
  lead: "Versioni {version}. Falas, me burim të hapur, pa llogari apo abonime."
  windows:
    title: "Windows"
    text: "Windows 10 dhe 11, 64-bit. Për përditësim, instaloni versionin e ri mbi të vjetrin."
    button: "Instaluesi (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ dhe shpërndarje të përputhshme."
    button: "Paketa (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ dhe shpërndarje të përputhshme."
    button: "Paketa (.rpm)"
  requirements: "Shprot përdor klientin OpenSSH (`ssh` dhe `ssh-keygen`). Ai vjen me Windows 10 / 11 dhe shumicën e desktopëve Linux; nëse mungon, Shprot ju tregon si ta shtoni."
  verify: "Çdo version përfshin një skedar `SHA256SUMS` për të kontrolluar që shkarkimi nuk është ndryshuar."
  all_releases: "Të gjitha versionet dhe ndryshimet"

faq:
  title: "Pyetje dhe përgjigje"
  items:
    - q: "A është Shprot VPN?"
      a: "Jo, është proxy. Vetëm aplikacionet e konfiguruara për ta përdorur — një shfletues, `curl`, `git`, një IDE — dërgojnë trafik përmes tunelit; gjithçka tjetër shkon drejtpërdrejt. Nuk instalohen përshtatës virtualë rrjeti."
    - q: "Çfarë më duhet në server?"
      a: "Një server i zakonshëm OpenSSH dhe një llogari që lejohet të përcjellë porta TCP. Nuk duhet instaluar asgjë në server; shërben çdo VPS ose server shtëpiak që arrihet me SSH."
    - q: "A mund të hyj me fjalëkalim?"
      a: "Jo, Shprot përdor vetëm çelësa SSH. Çelësat janë më të sigurt se fjalëkalimet dhe lejojnë tunelin të rilidhet pa ndërhyrjen tuaj. Çelësi krijohet në aplikacion me një klikim."
    - q: "Cilat aplikacione mund ta përdorin proxy-n?"
      a: "Shfletuesit, `curl`, `git`, menaxherët e paketave dhe çdo aplikacion me cilësime proxy SOCKS5 ose HTTP ose me mbështetje për ndryshoret `ALL_PROXY` / `HTTP_PROXY`. Përdorni `socks5h://` që emrat e faqeve të zgjidhen në server."
    - q: "Si e kontrollon Shprot tunelin?"
      a: "Herë pas here hap përmes proxy-t një faqe të njohur të zgjedhur rastësisht. Nëse dështon ndërkohë që interneti është i disponueshëm, tuneli riniset."
    - q: "A është falas?"
      a: "Po. Shprot është softuer i lirë dhe me burim të hapur me licencën GNU GPL v3, pa llogari, abonime apo reklama."
    - q: "A ka version për macOS?"
      a: "Ende jo. Aktualisht Shprot mbështet Windows 10 / 11 dhe Linux."

footer:
  license: "Shprot është softuer i lirë me licencën GNU GPL v3."
  source: "Kodi burimor"
  issues: "Raportoni një problem"
---
