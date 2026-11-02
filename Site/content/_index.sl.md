---
title: "Shprot — proxy SOCKS5 prek SSH, ki se sam znova poveže"
description: "Brezplačna aplikacija za Windows in Linux, ki ohranja delujoč proxy SOCKS5 in HTTP prek SSH: znova se poveže po menjavi omrežja in spanju, zazna zataknjene tunele, brez ukazne vrstice."
readme: "README.md"

ui:
  skip: "Skoči na vsebino"
  nav_features: "Zmožnosti"
  nav_compare: "Zakaj Shprot"
  nav_download: "Prenos"
  nav_faq: "Vprašanja"
  language: "Jezik"
  github: "Izvorna koda na GitHubu"

hero:
  title: "Proxy SOCKS5 prek SSH, ki ostane povezan"
  lead: "Shprot vsak strežnik, do katerega dostopate prek SSH, spremeni v stabilen proxy SOCKS5 in HTTP. Spremlja omrežje, obnovi tunel po menjavi Wi-Fi in prebujanju iz spanja ter vam natančno pove, kaj je šlo narobe — brez ukazne vrstice."
  download: "Prenesi"
  download_hint: "Različica {version} za Windows in Linux"
  facts:
    - "Brezplačno in odprtokodno"
    - "Windows 10 / 11 in Linux"
    - "49 jezikov"
  screenshot_alt: "Okno z nastavitvami programa Shprot"

features:
  title: "Vse, kar tunel potrebuje, da deluje naprej"
  items:
    - id: reconnect
      title: "Preživi menjavo omrežja"
      text: "Zamenjate Wi-Fi, priključite kabel ali zbudite prenosnik — Shprot to opazi in sam obnovi tunel."
    - id: health
      title: "Ujame tihe okvare"
      text: "Redna preverjanja prek samega proxyja najdejo tunel, ki je videti povezan, a ne prepušča prometa, in ga znova zaženejo."
    - id: gentle
      title: "Prizanesljiv do strežnika"
      text: "Ponovni poskusi so vse redkejši, po neuspelih prijavah pa se ponovijo največ enkrat na minuto, zato vam fail2ban zaradi napačnega ključa ne bo blokiral IP-naslova."
    - id: keys
      title: "Ključi SSH z dvema klikoma"
      text: "Ustvarite ali uvozite ključ in kopirajte njegov javni del za `authorized_keys` neposredno iz okna z nastavitvami."
    - id: trust
      title: "Jasno preverjanje strežnika"
      text: "Ob prvi povezavi se prstni odtis ključa strežnika prikaže v pogovornem oknu, da ga lahko preverite, preden strežniku zaupate."
    - id: http
      title: "Tudi proxy HTTP"
      text: "Programi brez podpore za SOCKS5 lahko uporabljajo izbirni proxy HTTP, ki teče skozi isti tunel."
    - id: tray
      title: "Živi v sistemski vrstici"
      text: "Ikona pokaže, ali tunel deluje, obvestila pojasnijo, zakaj ne, dnevnik pa je oddaljen en klik."
    - id: languages
      title: "Govori vaš jezik"
      text: "Vmesnik je preveden v 49 jezikov, za arabščino, perzijščino, hebrejščino in urdu pa z razporeditvijo od desne proti levi."

compare:
  title: "Zakaj ne kar `ssh -D`?"
  lead: "`ssh -D 1080 user@server` vam z enim ukazom da proxy SOCKS5 — dokler povezava tiho ne neha delovati. Shprot zažene isti odjemalec OpenSSH in poskrbi za vse, česar ukaz sam ne naredi."
  rows:
    - case: "Menjava omrežja ali Wi-Fi, prebujanje iz spanja"
      ssh: "Znova zaženite sami"
      shprot: "Samodejno se znova poveže"
    - case: "Tunel je povezan, a promet ne teče"
      ssh: "Ostane neopaženo"
      shprot: "Zaznajo ga preverjanja prek proxyja"
    - case: "Ponavljajoče se napake preverjanja pristnosti"
      ssh: "Odvisno od vaših skript"
      shprot: "Redki poskusi, da strežnik ne blokira vašega IP-naslova"
    - case: "Ključi odjemalca"
      ssh: "`ssh-keygen` in datoteke v `~/.ssh`"
      shprot: "Ustvarite ali uvozite, javni ključ kopirate z enim klikom"
    - case: "Preverjanje identitete strežnika"
      ssh: "Vprašanje v terminalu"
      shprot: "Pogovorno okno s prstnim odtisom ključa"
    - case: "Programi brez podpore za SOCKS5"
      ssh: "Niso podprti"
      shprot: "Izbirni proxy HTTP skozi isti tunel"
    - case: "Stanje tunela"
      ssh: "Okno terminala"
      shprot: "Ikona in obvestila z vzrokom napake"
  note: "Orodja, kot je `autossh`, pomagajo pri ponovnih zagonih, a še vedno zahtevajo ročno nastavitev in nimajo vmesnika."

uses:
  title: "Za kaj je primeren"
  items:
    - id: infra
      title: "Lastna infrastruktura"
      text: "Dostopajte do notranjih nadzornih plošč, testnih okolij in skrbniških orodij, ki so dosegljiva samo iz omrežja vašega strežnika."
    - id: wifi
      title: "Nezanesljiva omrežja"
      text: "Promet pošiljajte prek lastnega strežnika, ko uporabljate javni Wi-Fi v kavarnah, hotelih ali na letališčih."
    - id: laptop
      title: "Delo na daljavo s prenosnikom"
      text: "Stabilen proxy na računalniku, ki ves dan zaspi in menja omrežja."

how:
  title: "Pripravljeno v treh korakih"
  steps:
    - title: "Pripravite strežnik"
      text: "Ustreza vsak strežnik Linux z OpenSSH. Nanj se nič ne namešča — ločenega uporabnika, ki sme le posredovati vrata, ustvarite z nekaj ukazi."
    - title: "Povežite Shprot"
      text: "Vnesite `user@server`, ustvarite ključ in njegov javni del dodajte v `authorized_keys` na strežniku. Enkrat preverite prstni odtis strežnika."
    - title: "Usmerite programe na proxy"
      text: "Uporabite `socks5h://localhost:1080` v brskalniku ali sistemskih nastavitvah proxyja — meni v sistemski vrstici naslov kopira namesto vas."
  guide: "Navodila po korakih z zaščito strežnika"

tour:
  title: "Od blizu"
  items:
    - id: trust
      caption: "Ob prvi povezavi vidite prstni odtis ključa strežnika in se odločite, ali mu zaupate."
    - id: tray
      caption: "Meni v sistemski vrstici prikazuje stanje tunela in kopira pripravljene naslove proxyja."

download:
  title: "Prenesite Shprot"
  lead: "Različica {version}. Brezplačno, odprtokodno, brez računov in naročnin."
  windows:
    title: "Windows"
    text: "Windows 10 in 11, 64-bitni. Za posodobitev namestite novo različico čez staro."
    button: "Namestitveni program (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ in združljive distribucije."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ in združljive distribucije."
    button: "Paket (.rpm)"
  requirements: "Shprot uporablja odjemalec OpenSSH (`ssh` in `ssh-keygen`). Vključen je v Windows 10 / 11 in večino namizij Linux; če manjka, vam Shprot pove, kako ga dodati."
  verify: "Vsaka izdaja vsebuje datoteko `SHA256SUMS`, s katero preverite, da prenesena datoteka ni bila spremenjena."
  all_releases: "Vse izdaje in spremembe"

faq:
  title: "Vprašanja in odgovori"
  items:
    - q: "Ali je Shprot VPN?"
      a: "Ne, je proxy. Skozi tunel pošiljajo promet le programi, ki so nastavljeni za njegovo uporabo — brskalnik, `curl`, `git`, IDE; vse drugo gre neposredno. Navidezni omrežni vmesniki se ne namestijo."
    - q: "Kaj potrebujem na strežniku?"
      a: "Običajen strežnik OpenSSH in račun, ki sme posredovati vrata TCP. Na strežnik ni treba ničesar nameščati; ustreza vsak VPS ali domači strežnik, dosegljiv prek SSH."
    - q: "Ali se lahko prijavim z geslom?"
      a: "Ne, Shprot uporablja samo ključe SSH. Ključi so varnejši od gesel in tunelu omogočajo, da se znova poveže brez vašega posega. Ključ ustvarite v aplikaciji z enim klikom."
    - q: "Kateri programi lahko uporabljajo proxy?"
      a: "Brskalniki, `curl`, `git`, upravitelji paketov in vsak program z nastavitvami proxyja SOCKS5 ali HTTP ali s podporo za spremenljivki `ALL_PROXY` / `HTTP_PROXY`. Uporabite `socks5h://`, da se imena spletnih mest razrešujejo na strežniku."
    - q: "Kako Shprot preverja tunel?"
      a: "Občasno prek proxyja odpre naključno izbrano priljubljeno spletno mesto. Če to ne uspe, čeprav je internet dosegljiv, se tunel znova zažene."
    - q: "Ali je brezplačen?"
      a: "Da. Shprot je prosto in odprtokodno programje z licenco GNU GPL v3, brez računov, naročnin in oglasov."
    - q: "Ali obstaja različica za macOS?"
      a: "Še ne. Shprot trenutno podpira Windows 10 / 11 in Linux."

footer:
  license: "Shprot je prosto programje z licenco GNU GPL v3."
  source: "Izvorna koda"
  issues: "Prijavite težavo"
---
