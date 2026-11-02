---
title: "Shprot — SOCKS5 starpniekserveris caur SSH, kas pats savienojas no jauna"
description: "Bezmaksas lietotne Windows un Linux, kas uztur darbībā SOCKS5 un HTTP starpniekserveri caur SSH: savienojas no jauna pēc tīkla maiņas un miega, pamana iestrēgušus tuneļus, bez komandrindas."
readme: "README.md"

ui:
  skip: "Pāriet uz saturu"
  nav_features: "Iespējas"
  nav_compare: "Kāpēc Shprot"
  nav_download: "Lejupielādēt"
  nav_faq: "BUJ"
  language: "Valoda"
  github: "Pirmkods GitHub"

hero:
  title: "SOCKS5 starpniekserveris caur SSH, kas paliek savienots"
  lead: "Shprot pārvērš jebkuru serveri, kam piekļūstat ar SSH, par stabilu SOCKS5 un HTTP starpniekserveri. Tas uzrauga tīklu, atjauno tuneli pēc Wi-Fi maiņas un pamošanās no miega un precīzi pasaka, kas nogāja greizi — bez komandrindas."
  download: "Lejupielādēt"
  download_hint: "Versija {version} Windows un Linux"
  facts:
    - "Bezmaksas un atvērtā koda"
    - "Windows 10 / 11 un Linux"
    - "49 valodas"
  screenshot_alt: "Shprot iestatījumu logs"

features:
  title: "Viss, kas tunelim vajadzīgs, lai turpinātu darboties"
  items:
    - id: reconnect
      title: "Iztur tīkla maiņu"
      text: "Nomainījāt Wi-Fi, pievienojāt kabeli vai pamodinājāt klēpjdatoru — Shprot to pamana un pats atjauno tuneli."
    - id: health
      title: "Notver klusas kļūmes"
      text: "Regulāras pārbaudes caur pašu starpniekserveri atrod tuneli, kas izskatās savienots, bet nelaiž cauri datplūsmu, un to restartē."
    - id: gentle
      title: "Saudzē jūsu serveri"
      text: "Atkārtoti mēģinājumi pamazām kļūst retāki, un pēc neveiksmīgām pieteikšanās reizēm tie notiek ne biežāk kā reizi minūtē, tāpēc fail2ban nebloķēs jūsu IP nepareizas atslēgas dēļ."
    - id: keys
      title: "SSH atslēgas ar diviem klikšķiem"
      text: "Izveidojiet vai importējiet atslēgu un nokopējiet tās publisko daļu failam `authorized_keys` tieši no iestatījumu loga."
    - id: trust
      title: "Skaidra servera pārbaude"
      text: "Pirmajā savienojumā servera atslēgas pirkstu nospiedums tiek parādīts dialoglodziņā, lai jūs to varētu pārbaudīt, pirms uzticaties serverim."
    - id: http
      title: "Arī HTTP starpniekserveris"
      text: "Lietotnes bez SOCKS5 atbalsta var izmantot papildu HTTP starpniekserveri, kas darbojas caur to pašu tuneli."
    - id: tray
      title: "Dzīvo sistēmas teknē"
      text: "Ikona rāda, vai tunelis ir atvērts, paziņojumi paskaidro, kāpēc nav, un žurnāls ir viena klikšķa attālumā."
    - id: languages
      title: "Runā jūsu valodā"
      text: "Saskarne ir tulkota 49 valodās, arābu, persiešu, ivrita un urdu valodai ar izkārtojumu no labās uz kreiso."

compare:
  title: "Kāpēc ne vienkārši `ssh -D`?"
  lead: "`ssh -D 1080 user@server` ar vienu komandu dod SOCKS5 starpniekserveri — līdz savienojums nemanāmi pārstāj darboties. Shprot palaiž to pašu OpenSSH klientu un parūpējas par visu, ko komanda viena pati nedara."
  rows:
    - case: "Tīkla vai Wi-Fi maiņa, pamošanās no miega"
      ssh: "Restartējiet paši"
      shprot: "Automātiski savienojas no jauna"
    - case: "Tunelis savienots, bet datplūsma nenotiek"
      ssh: "Paliek nepamanīts"
      shprot: "Atklāj pārbaudes caur starpniekserveri"
    - case: "Atkārtotas autentifikācijas kļūdas"
      ssh: "Atkarīgs no jūsu skriptiem"
      shprot: "Reti mēģinājumi, lai serveris nebloķētu jūsu IP"
    - case: "Klienta atslēgas"
      ssh: "`ssh-keygen` un faili mapē `~/.ssh`"
      shprot: "Izveidot vai importēt, publisko atslēgu nokopēt ar vienu klikšķi"
    - case: "Servera identitātes pārbaude"
      ssh: "Jautājums terminālī"
      shprot: "Dialoglodziņš ar atslēgas pirkstu nospiedumu"
    - case: "Lietotnes bez SOCKS5 atbalsta"
      ssh: "Netiek atbalstītas"
      shprot: "Papildu HTTP starpniekserveris caur to pašu tuneli"
    - case: "Tuneļa stāvoklis"
      ssh: "Termināļa logs"
      shprot: "Ikona un paziņojumi ar kļūmes iemeslu"
  note: "Rīki, piemēram, `autossh`, palīdz ar restartēšanu, taču joprojām prasa manuālu konfigurēšanu un tiem nav saskarnes."

uses:
  title: "Kam tas noder"
  items:
    - id: infra
      title: "Jūsu infrastruktūra"
      text: "Piekļūstiet iekšējiem paneļiem, testa vidēm un administrēšanas rīkiem, kas pieejami tikai no jūsu servera tīkla."
    - id: wifi
      title: "Neuzticami tīkli"
      text: "Sūtiet datplūsmu caur savu serveri, kad izmantojat publisko Wi-Fi kafejnīcās, viesnīcās vai lidostās."
    - id: laptop
      title: "Attālināts darbs ar klēpjdatoru"
      text: "Stabils starpniekserveris datorā, kas visu dienu iemieg un maina tīklus."

how:
  title: "Gatavs trīs soļos"
  steps:
    - title: "Sagatavojiet serveri"
      text: "Der jebkurš Linux serveris ar OpenSSH. Tajā nekas netiek instalēts — atsevišķu lietotāju, kam atļauts tikai pāradresēt portus, izveido ar dažām komandām."
    - title: "Savienojiet Shprot"
      text: "Ievadiet `user@server`, izveidojiet atslēgu un pievienojiet tās publisko daļu servera failam `authorized_keys`. Vienreiz pārbaudiet servera pirkstu nospiedumu."
    - title: "Novirziet lietotnes uz starpniekserveri"
      text: "Izmantojiet `socks5h://localhost:1080` pārlūkā vai sistēmas starpniekservera iestatījumos — teknes izvēlne nokopēs adresi jūsu vietā."
  guide: "Soli pa solim ceļvedis ar servera aizsardzību"

tour:
  title: "Tuvāk"
  items:
    - id: trust
      caption: "Pirmajā savienojumā jūs redzat servera atslēgas pirkstu nospiedumu un izlemjat, vai tam uzticēties."
    - id: tray
      caption: "Teknes izvēlne rāda tuneļa stāvokli un kopē lietošanai gatavas starpniekservera adreses."

download:
  title: "Lejupielādēt Shprot"
  lead: "Versija {version}. Bezmaksas, atvērtā koda, bez kontiem un abonementiem."
  windows:
    title: "Windows"
    text: "Windows 10 un 11, 64 bitu. Lai atjauninātu, instalējiet jauno versiju virs vecās."
    button: "Instalētājs (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ un saderīgi izplatījumi."
    button: "Pakotne (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ un saderīgi izplatījumi."
    button: "Pakotne (.rpm)"
  requirements: "Shprot izmanto OpenSSH klientu (`ssh` un `ssh-keygen`). Tas ir iekļauts Windows 10 / 11 un vairumā Linux darbvirsmu; ja tā nav, Shprot pastāstīs, kā to pievienot."
  verify: "Katrā versijā ir fails `SHA256SUMS`, ar kuru varat pārbaudīt, vai lejupielādētais fails nav mainīts."
  all_releases: "Visas versijas un izmaiņas"

faq:
  title: "Jautājumi un atbildes"
  items:
    - q: "Vai Shprot ir VPN?"
      a: "Nē, tas ir starpniekserveris. Caur tuneli datplūsmu sūta tikai lietotnes, kas iestatītas to izmantot — pārlūks, `curl`, `git`, IDE; viss pārējais iet tieši. Virtuālie tīkla adapteri netiek instalēti."
    - q: "Kas vajadzīgs serverī?"
      a: "Parasts OpenSSH serveris un konts, kuram atļauts pāradresēt TCP portus. Serverī nekas nav jāinstalē; der jebkurš ar SSH sasniedzams VPS vai mājas serveris."
    - q: "Vai varu pieteikties ar paroli?"
      a: "Nē, Shprot izmanto tikai SSH atslēgas. Atslēgas ir drošākas par parolēm un ļauj tunelim savienoties no jauna bez jūsu iejaukšanās. Atslēgu lietotnē izveido ar vienu klikšķi."
    - q: "Kuras lietotnes var izmantot starpniekserveri?"
      a: "Pārlūki, `curl`, `git`, pakotņu pārvaldnieki un jebkura lietotne ar SOCKS5 vai HTTP starpniekservera iestatījumiem vai mainīgo `ALL_PROXY` / `HTTP_PROXY` atbalstu. Izmantojiet `socks5h://`, lai vietņu nosaukumi tiktu atrisināti serverī."
    - q: "Kā Shprot pārbauda tuneli?"
      a: "Laiku pa laikam tas caur starpniekserveri atver nejauši izvēlētu populāru vietni. Ja tas neizdodas, lai gan internets ir pieejams, tunelis tiek restartēts."
    - q: "Vai tas ir bezmaksas?"
      a: "Jā. Shprot ir brīva atvērtā koda programmatūra ar GNU GPL v3 licenci, bez kontiem, abonementiem un reklāmām."
    - q: "Vai ir macOS versija?"
      a: "Vēl nav. Pašlaik Shprot atbalsta Windows 10 / 11 un Linux."

footer:
  license: "Shprot ir brīva programmatūra ar GNU GPL v3 licenci."
  source: "Pirmkods"
  issues: "Ziņot par problēmu"
---
