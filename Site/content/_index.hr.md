---
title: "Shprot — SOCKS5 proxy preko SSH-a koji se sam ponovno povezuje"
description: "Besplatna aplikacija za Windows i Linux koja održava SOCKS5 i HTTP proxy preko SSH-a: ponovno se povezuje nakon promjene mreže i stanja mirovanja, otkriva zaglavljene tunele, bez naredbenog retka."
readme: "README.md"

ui:
  skip: "Prijeđi na sadržaj"
  nav_features: "Značajke"
  nav_compare: "Zašto Shprot"
  nav_download: "Preuzmi"
  nav_faq: "Pitanja"
  language: "Jezik"
  github: "Izvorni kod na GitHubu"

hero:
  title: "SOCKS5 proxy preko SSH-a koji ostaje povezan"
  lead: "Shprot pretvara bilo koji poslužitelj kojem pristupate preko SSH-a u stabilan SOCKS5 i HTTP proxy. Prati mrežu, vraća tunel nakon promjene Wi-Fi mreže i buđenja iz mirovanja te vam točno kaže što nije u redu — bez naredbenog retka."
  download: "Preuzmi"
  download_hint: "Verzija {version} za Windows i Linux"
  facts:
    - "Besplatno i otvorenog koda"
    - "Windows 10 / 11 i Linux"
    - "49 jezika"
  screenshot_alt: "Prozor postavki programa Shprot"

features:
  title: "Sve što tunelu treba da nastavi raditi"
  items:
    - id: reconnect
      title: "Preživljava promjenu mreže"
      text: "Promijenite Wi-Fi, priključite kabel ili probudite laptop — Shprot to primijeti i sam vrati tunel."
    - id: health
      title: "Hvata tihe kvarove"
      text: "Redovite provjere kroz sam proxy pronalaze tunel koji izgleda povezan, ali ne propušta promet, i ponovno ga pokreću."
    - id: gentle
      title: "Štedi vaš poslužitelj"
      text: "Ponovni pokušaji postupno se prorjeđuju, a nakon neuspjelih prijava ponavljaju se najviše jednom u minuti, pa vam fail2ban neće blokirati IP adresu zbog pogrešnog ključa."
    - id: keys
      title: "SSH ključevi u dva klika"
      text: "Stvorite ili uvezite ključ i kopirajte javni dio za `authorized_keys` izravno iz prozora postavki."
    - id: trust
      title: "Jasna provjera poslužitelja"
      text: "Pri prvom povezivanju otisak ključa poslužitelja prikazuje se u dijalogu, pa ga možete provjeriti prije nego što poslužitelju ukažete povjerenje."
    - id: http
      title: "I HTTP proxy"
      text: "Programi bez podrške za SOCKS5 mogu koristiti dodatni HTTP proxy koji ide kroz isti tunel."
    - id: tray
      title: "Živi u traci sustava"
      text: "Ikona pokazuje je li tunel otvoren, obavijesti objašnjavaju zašto nije, a zapisnik je na jedan klik."
    - id: languages
      title: "Govori vaš jezik"
      text: "Sučelje je prevedeno na 49 jezika, s rasporedom zdesna nalijevo za arapski, perzijski, hebrejski i urdu."

compare:
  title: "Zašto ne samo `ssh -D`?"
  lead: "`ssh -D 1080 user@server` daje vam SOCKS5 proxy jednom naredbom — sve dok veza neprimjetno ne prestane raditi. Shprot pokreće isti OpenSSH klijent i brine o svemu što sama naredba ne radi."
  rows:
    - case: "Promjena mreže ili Wi-Fi-ja, buđenje iz mirovanja"
      ssh: "Ponovno pokrenite sami"
      shprot: "Automatski se ponovno povezuje"
    - case: "Tunel je povezan, ali promet ne prolazi"
      ssh: "Prolazi neopaženo"
      shprot: "Otkrivaju ga provjere kroz proxy"
    - case: "Ponovljene pogreške autentifikacije"
      ssh: "Ovisi o vašim skriptama"
      shprot: "Rijetki pokušaji, da poslužitelj ne blokira vašu IP adresu"
    - case: "Klijentski ključevi"
      ssh: "`ssh-keygen` i datoteke u `~/.ssh`"
      shprot: "Stvorite ili uvezite, kopirajte javni ključ jednim klikom"
    - case: "Provjera identiteta poslužitelja"
      ssh: "Pitanje u terminalu"
      shprot: "Dijalog s otiskom ključa poslužitelja"
    - case: "Programi bez podrške za SOCKS5"
      ssh: "Nisu pokriveni"
      shprot: "Dodatni HTTP proxy kroz isti tunel"
    - case: "Stanje tunela"
      ssh: "Prozor terminala"
      shprot: "Ikona i obavijesti s razlogom pogreške"
  note: "Alati poput `autossh` pomažu pri ponovnom pokretanju, ali i dalje zahtijevaju ručno postavljanje i nemaju sučelje."

uses:
  title: "Za što je dobar"
  items:
    - id: infra
      title: "Vlastita infrastruktura"
      text: "Pristupite internim nadzornim pločama, testnim okruženjima i administratorskim alatima dostupnima samo iz mreže vašeg poslužitelja."
    - id: wifi
      title: "Nepouzdane mreže"
      text: "Šaljite promet kroz vlastiti poslužitelj kada koristite javni Wi-Fi u kafićima, hotelima ili zračnim lukama."
    - id: laptop
      title: "Rad na daljinu s laptopom"
      text: "Stabilan proxy na računalu koje cijeli dan ide u mirovanje i mijenja mreže."

how:
  title: "Spremno u tri koraka"
  steps:
    - title: "Pripremite poslužitelj"
      text: "Odgovara bilo koji Linux poslužitelj s OpenSSH-om. Na njega se ništa ne instalira — zaseban korisnik kojem je dopušteno samo prosljeđivanje priključaka stvara se s nekoliko naredbi."
    - title: "Povežite Shprot"
      text: "Unesite `user@server`, stvorite ključ i dodajte njegov javni dio u `authorized_keys` na poslužitelju. Jednom provjerite otisak poslužitelja."
    - title: "Usmjerite programe na proxy"
      text: "Upotrijebite `socks5h://localhost:1080` u pregledniku ili postavkama proxyja sustava — izbornik u traci sustava kopira adresu umjesto vas."
  guide: "Upute korak po korak sa zaštitom poslužitelja"

tour:
  title: "Izbliza"
  items:
    - id: trust
      caption: "Pri prvom povezivanju vidite otisak ključa poslužitelja i odlučujete hoćete li mu vjerovati."
    - id: tray
      caption: "Izbornik u traci sustava prikazuje stanje tunela i kopira spremne adrese proxyja."

download:
  title: "Preuzmite Shprot"
  lead: "Verzija {version}. Besplatno, otvorenog koda, bez računa i pretplata."
  windows:
    title: "Windows"
    text: "Windows 10 i 11, 64-bitni. Za ažuriranje instalirajte novu verziju preko stare."
    button: "Instalacijski program (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ i kompatibilne distribucije."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ i kompatibilne distribucije."
    button: "Paket (.rpm)"
  requirements: "Shprot koristi OpenSSH klijent (`ssh` i `ssh-keygen`). Dolazi uz Windows 10 / 11 i većinu Linux okruženja; ako nedostaje, Shprot će vam reći kako ga dodati."
  verify: "Svaka verzija sadrži datoteku `SHA256SUMS` kojom možete provjeriti da preuzeta datoteka nije izmijenjena."
  all_releases: "Sve verzije i promjene"

faq:
  title: "Pitanja i odgovori"
  items:
    - q: "Je li Shprot VPN?"
      a: "Ne, to je proxy. Kroz tunel ide promet samo programa postavljenih da ga koriste — preglednika, `curl`, `git`, IDE-a; sve ostalo ide izravno. Ne instaliraju se virtualni mrežni prilagodnici."
    - q: "Što mi treba na poslužitelju?"
      a: "Običan OpenSSH poslužitelj i račun kojem je dopušteno prosljeđivanje TCP priključaka. Na poslužitelj ne treba ništa instalirati; odgovara svaki VPS ili kućni poslužitelj dostupan preko SSH-a."
    - q: "Mogu li se prijavljivati lozinkom?"
      a: "Ne, Shprot koristi samo SSH ključeve. Ključevi su sigurniji od lozinki i omogućuju tunelu da se ponovno poveže bez vaše intervencije. Ključ se stvara u aplikaciji jednim klikom."
    - q: "Koji programi mogu koristiti proxy?"
      a: "Preglednici, `curl`, `git`, upravitelji paketa i svaki program s postavkama SOCKS5 ili HTTP proxyja ili podrškom za varijable `ALL_PROXY` / `HTTP_PROXY`. Koristite `socks5h://` kako bi se nazivi web-mjesta razrješavali na poslužitelju."
    - q: "Kako Shprot provjerava tunel?"
      a: "S vremena na vrijeme kroz proxy otvori nasumično odabrano popularno web-mjesto. Ako to ne uspije dok je internet dostupan, tunel se ponovno pokreće."
    - q: "Je li besplatan?"
      a: "Da. Shprot je slobodan softver otvorenog koda pod licencom GNU GPL v3, bez računa, pretplata i oglasa."
    - q: "Postoji li verzija za macOS?"
      a: "Još ne. Trenutačno Shprot podržava Windows 10 / 11 i Linux."

footer:
  license: "Shprot je slobodan softver pod licencom GNU GPL v3."
  source: "Izvorni kod"
  issues: "Prijavite problem"
---
