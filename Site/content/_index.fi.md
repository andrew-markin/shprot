---
title: "Shprot — SOCKS5-välityspalvelin SSH:n yli, joka yhdistää itse uudelleen"
description: "Ilmainen työpöytäsovellus Windowsille ja Linuxille, joka pitää SOCKS5- ja HTTP-välityspalvelimen SSH:n yli toiminnassa: yhdistää uudelleen verkon vaihdon ja lepotilan jälkeen, havaitsee jumittuneet tunnelit, ilman komentoriviä."
readme: "README.md"

ui:
  skip: "Siirry sisältöön"
  nav_features: "Ominaisuudet"
  nav_compare: "Miksi Shprot"
  nav_download: "Lataa"
  nav_faq: "Kysymykset"
  language: "Kieli"
  github: "Lähdekoodi GitHubissa"

hero:
  title: "SOCKS5-välityspalvelin SSH:n yli, joka pysyy yhteydessä"
  lead: "Shprot tekee mistä tahansa SSH:lla tavoitettavasta palvelimesta vakaan SOCKS5- ja HTTP-välityspalvelimen. Se tarkkailee verkkoa, palauttaa tunnelin Wi-Fin vaihdon ja lepotilan jälkeen ja kertoo tarkalleen, mikä meni vikaan — ilman komentoriviä."
  download: "Lataa"
  download_hint: "Versio {version} Windowsille ja Linuxille"
  facts:
    - "Ilmainen ja avointa lähdekoodia"
    - "Windows 10 / 11 ja Linux"
    - "49 kieltä"
  screenshot_alt: "Shprotin asetusikkuna"

features:
  title: "Kaikki, mitä tunneli tarvitsee pysyäkseen toiminnassa"
  items:
    - id: reconnect
      title: "Kestää verkon vaihdot"
      text: "Wi-Fi vaihtuu, kaapeli kytketään tai kannettava herää lepotilasta — Shprot huomaa sen ja palauttaa tunnelin itse."
    - id: health
      title: "Havaitsee hiljaiset viat"
      text: "Säännölliset tarkistukset itse välityspalvelimen kautta löytävät tunnelin, joka näyttää yhdistetyltä mutta ei päästä liikennettä läpi, ja käynnistävät sen uudelleen."
    - id: gentle
      title: "Säästää palvelintasi"
      text: "Uudet yritykset harvenevat vähitellen, ja epäonnistuneiden kirjautumisten jälkeen niitä tehdään enintään kerran minuutissa, joten fail2ban ei estä IP-osoitettasi väärän avaimen takia."
    - id: keys
      title: "SSH-avaimet kahdella napsautuksella"
      text: "Luo tai tuo avain ja kopioi sen julkinen osa `authorized_keys`-tiedostoon suoraan asetusikkunasta."
    - id: trust
      title: "Selkeä palvelimen tarkistus"
      text: "Ensimmäisellä yhteyskerralla palvelimen isäntäavaimen sormenjälki näytetään valintaikkunassa, joten voit tarkistaa sen ennen kuin luotat palvelimeen."
    - id: http
      title: "Myös HTTP-välityspalvelin"
      text: "Sovellukset, jotka eivät tue SOCKS5:tä, voivat käyttää valinnaista HTTP-välityspalvelinta, joka kulkee saman tunnelin kautta."
    - id: tray
      title: "Asuu ilmoitusalueella"
      text: "Kuvake näyttää, onko tunneli auki, ilmoitukset kertovat miksi ei, ja lokit ovat yhden napsautuksen päässä."
    - id: languages
      title: "Puhuu kieltäsi"
      text: "Käyttöliittymä on käännetty 49 kielelle, ja arabialle, persialle, heprealle ja urdulle käytetään oikealta vasemmalle -asettelua."

compare:
  title: "Miksei vain `ssh -D`?"
  lead: "`ssh -D 1080 user@server` antaa SOCKS5-välityspalvelimen yhdellä komennolla — kunnes yhteys lakkaa huomaamatta toimimasta. Shprot ajaa samaa OpenSSH-asiakasta ja hoitaa kaiken, mitä pelkkä komento ei tee."
  rows:
    - case: "Verkon tai Wi-Fin vaihto, herääminen lepotilasta"
      ssh: "Käynnistä itse uudelleen"
      shprot: "Yhdistää automaattisesti uudelleen"
    - case: "Tunneli yhdistetty, mutta liikenne ei kulje"
      ssh: "Jää huomaamatta"
      shprot: "Havaitaan välityspalvelimen kautta tehtävillä tarkistuksilla"
    - case: "Toistuvat tunnistautumisvirheet"
      ssh: "Omien skriptiesi varassa"
      shprot: "Harvat yritykset, jottei palvelin estä IP-osoitettasi"
    - case: "Asiakkaan avaimet"
      ssh: "`ssh-keygen` ja tiedostot hakemistossa `~/.ssh`"
      shprot: "Luo tai tuo, kopioi julkinen avain yhdellä napsautuksella"
    - case: "Palvelimen identiteetin tarkistus"
      ssh: "Kysymys päätteessä"
      shprot: "Valintaikkuna isäntäavaimen sormenjäljellä"
    - case: "Sovellukset ilman SOCKS5-tukea"
      ssh: "Ei katettu"
      shprot: "Valinnainen HTTP-välityspalvelin saman tunnelin kautta"
    - case: "Tunnelin tila"
      ssh: "Pääteikkuna"
      shprot: "Ilmoitusalueen kuvake ja ilmoitukset vian syystä"
  note: "Työkalut kuten `autossh` auttavat uudelleenkäynnistyksissä, mutta vaativat silti käsin tehtävät asetukset eikä niissä ole käyttöliittymää."

uses:
  title: "Mihin se sopii"
  items:
    - id: infra
      title: "Oma infrastruktuuri"
      text: "Pääse sisäisiin hallintapaneeleihin, testiympäristöihin ja ylläpitotyökaluihin, jotka ovat saatavilla vain palvelimesi verkosta."
    - id: wifi
      title: "Epäluotettavat verkot"
      text: "Ohjaa liikenteesi oman palvelimesi kautta, kun käytät julkista Wi-Fiä kahviloissa, hotelleissa tai lentokentillä."
    - id: laptop
      title: "Etätyö kannettavalla"
      text: "Vakaa välityspalvelin koneessa, joka menee lepotilaan ja vaihtaa verkkoa koko päivän."

how:
  title: "Käyttövalmis kolmessa vaiheessa"
  steps:
    - title: "Valmistele palvelin"
      text: "Mikä tahansa Linux-palvelin, jossa on OpenSSH, käy. Siihen ei asenneta mitään — oma käyttäjä, joka saa vain välittää portteja, luodaan muutamalla komennolla."
    - title: "Yhdistä Shprot"
      text: "Anna `user@server`, luo avain ja lisää sen julkinen osa palvelimen `authorized_keys`-tiedostoon. Tarkista palvelimen sormenjälki kerran."
    - title: "Ohjaa sovellukset välityspalvelimelle"
      text: "Käytä osoitetta `socks5h://localhost:1080` selaimessa tai järjestelmän välityspalvelinasetuksissa — ilmoitusalueen valikko kopioi osoitteen puolestasi."
  guide: "Vaiheittainen ohje palvelimen suojaamiseen"

tour:
  title: "Tarkempi katsaus"
  items:
    - id: trust
      caption: "Ensimmäisellä yhteyskerralla näet palvelimen avaimen sormenjäljen ja päätät, luotatko siihen."
    - id: tray
      caption: "Ilmoitusalueen valikko näyttää tunnelin tilan ja kopioi valmiit välityspalvelinosoitteet."

download:
  title: "Lataa Shprot"
  lead: "Versio {version}. Ilmainen, avointa lähdekoodia, ei tilejä eikä tilauksia."
  windows:
    title: "Windows"
    text: "Windows 10 ja 11, 64-bittinen. Päivitä asentamalla uusi versio vanhan päälle."
    button: "Asennusohjelma (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ ja yhteensopivat jakelut."
    button: "Paketti (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ ja yhteensopivat jakelut."
    button: "Paketti (.rpm)"
  requirements: "Shprot käyttää OpenSSH-asiakasta (`ssh` ja `ssh-keygen`). Se sisältyy Windows 10 / 11:een ja useimpiin Linux-työpöytiin; jos se puuttuu, Shprot kertoo, miten sen voi lisätä."
  verify: "Jokaisessa versiossa on `SHA256SUMS`-tiedosto, jolla voit tarkistaa, ettei ladattua tiedostoa ole muutettu."
  all_releases: "Kaikki versiot ja muutokset"

faq:
  title: "Kysymyksiä ja vastauksia"
  items:
    - q: "Onko Shprot VPN?"
      a: "Ei, se on välityspalvelin. Vain sen käyttöön määritetyt sovellukset — selain, `curl`, `git`, IDE — lähettävät liikennettä tunnelin kautta; kaikki muu kulkee suoraan. Virtuaalisia verkkosovittimia ei asenneta."
    - q: "Mitä palvelimella tarvitaan?"
      a: "Tavallinen OpenSSH-palvelin ja tili, jolla on oikeus välittää TCP-portteja. Palvelimelle ei tarvitse asentaa mitään; mikä tahansa SSH:lla tavoitettava VPS tai kotipalvelin käy."
    - q: "Voinko kirjautua salasanalla?"
      a: "Et, Shprot käyttää vain SSH-avaimia. Avaimet ovat salasanoja turvallisempia ja antavat tunnelin yhdistää uudelleen ilman sinua. Avaimen voi luoda sovelluksessa yhdellä napsautuksella."
    - q: "Mitkä sovellukset voivat käyttää välityspalvelinta?"
      a: "Selaimet, `curl`, `git`, paketinhallinnat ja kaikki sovellukset, joissa on SOCKS5- tai HTTP-välityspalvelimen asetukset tai tuki muuttujille `ALL_PROXY` / `HTTP_PROXY`. Käytä `socks5h://`-osoitetta, jotta sivustojen nimet selvitetään palvelimella."
    - q: "Miten Shprot tarkistaa tunnelin?"
      a: "Aika ajoin se avaa välityspalvelimen kautta satunnaisesti valitun suositun verkkosivuston. Jos se epäonnistuu, vaikka internet on käytettävissä, tunneli käynnistetään uudelleen."
    - q: "Onko se ilmainen?"
      a: "On. Shprot on vapaata ja avointa lähdekoodia olevaa ohjelmistoa GNU GPL v3 -lisenssillä, ilman tilejä, tilauksia tai mainoksia."
    - q: "Onko macOS-versiota?"
      a: "Ei vielä. Shprot tukee tällä hetkellä Windows 10 / 11:tä ja Linuxia."

footer:
  license: "Shprot on vapaata ohjelmistoa GNU GPL v3 -lisenssillä."
  source: "Lähdekoodi"
  issues: "Ilmoita ongelmasta"
---
