---
title: "Shprot — SSH kaudu töötav SOCKS5-puhverserver, mis taastab ühenduse ise"
description: "Tasuta töölauarakendus Windowsile ja Linuxile, mis hoiab SSH kaudu töötava SOCKS5- ja HTTP-puhverserveri käigus: taastab ühenduse pärast võrgu vahetust ja unerežiimi, märkab kinni jäänud tunneleid, ilma käsureata."
readme: "README.md"

ui:
  skip: "Mine sisu juurde"
  nav_features: "Võimalused"
  nav_compare: "Miks Shprot"
  nav_download: "Laadi alla"
  nav_faq: "KKK"
  language: "Keel"
  github: "Lähtekood GitHubis"

hero:
  title: "SSH kaudu töötav SOCKS5-puhverserver, mis ei katkesta"
  lead: "Shprot muudab iga serveri, millele SSH kaudu ligi pääsete, stabiilseks SOCKS5- ja HTTP-puhverserveriks. See jälgib võrku, taastab tunneli pärast WiFi vahetust ja unerežiimist ärkamist ning ütleb täpselt, mis valesti läks — ilma käsureata."
  download: "Laadi alla"
  download_hint: "Versioon {version} Windowsile ja Linuxile"
  facts:
    - "Tasuta ja avatud lähtekoodiga"
    - "Windows 10 / 11 ja Linux"
    - "49 keelt"
  screenshot_alt: "Shproti seadete aken"

features:
  title: "Kõik, mida tunnel vajab, et töös püsida"
  items:
    - id: reconnect
      title: "Elab üle võrgu vahetuse"
      text: "Vahetasite WiFi-võrku, ühendasite kaabli või äratasite sülearvuti — Shprot märkab seda ja taastab tunneli ise."
    - id: health
      title: "Tabab vaikseid tõrkeid"
      text: "Regulaarsed kontrollid puhverserveri enda kaudu leiavad tunneli, mis paistab ühendatud, kuid ei lase liiklust läbi, ja käivitavad selle uuesti."
    - id: gentle
      title: "Säästab teie serverit"
      text: "Uued katsed hõrenevad järk-järgult ja pärast ebaõnnestunud sisselogimisi korratakse neid kõige rohkem kord minutis, nii et fail2ban ei blokeeri teie IP-d vale võtme pärast."
    - id: keys
      title: "SSH-võtmed kahe klõpsuga"
      text: "Looge või importige võti ja kopeerige selle avalik osa faili `authorized_keys` jaoks otse seadete aknast."
    - id: trust
      title: "Selge serveri kontroll"
      text: "Esimesel ühendusel näidatakse serveri hostivõtme sõrmejälge dialoogis, et saaksite seda enne serverit usaldamast kontrollida."
    - id: http
      title: "Ka HTTP-puhverserver"
      text: "SOCKS5-toeta rakendused saavad kasutada valikulist HTTP-puhverserverit, mis kulgeb läbi sama tunneli."
    - id: tray
      title: "Elab teavitusalal"
      text: "Ikoon näitab, kas tunnel on avatud, teavitused selgitavad, miks mitte, ja logid on ühe klõpsu kaugusel."
    - id: languages
      title: "Räägib teie keelt"
      text: "Liides on tõlgitud 49 keelde, araabia, pärsia, heebrea ja urdu keele jaoks paremalt vasakule paigutusega."

compare:
  title: "Miks mitte lihtsalt `ssh -D`?"
  lead: "`ssh -D 1080 user@server` annab SOCKS5-puhverserveri ühe käsuga — kuni ühendus vaikselt töötamast lakkab. Shprot käivitab sama OpenSSH kliendi ja hoolitseb kõige eest, mida käsk üksi ei tee."
  rows:
    - case: "Võrgu või WiFi vahetus, unerežiimist ärkamine"
      ssh: "Taaskäivitage ise"
      shprot: "Taastab ühenduse automaatselt"
    - case: "Tunnel on ühendatud, kuid liiklust ei toimu"
      ssh: "Jääb märkamata"
      shprot: "Avastatakse puhverserveri kaudu tehtavate kontrollidega"
    - case: "Korduvad autentimisvead"
      ssh: "Teie skriptide hooleks"
      shprot: "Harvad katsed, et server teie IP-d ei blokeeriks"
    - case: "Kliendi võtmed"
      ssh: "`ssh-keygen` ja failid kaustas `~/.ssh`"
      shprot: "Loo või impordi, kopeeri avalik võti ühe klõpsuga"
    - case: "Serveri identiteedi kontroll"
      ssh: "Küsimus terminalis"
      shprot: "Dialoog hostivõtme sõrmejäljega"
    - case: "SOCKS5-toeta rakendused"
      ssh: "Ei ole kaetud"
      shprot: "Valikuline HTTP-puhverserver sama tunneli kaudu"
    - case: "Tunneli olek"
      ssh: "Terminaliaken"
      shprot: "Ikoon teavitusalal ja teavitused tõrke põhjusega"
  note: "Tööriistad nagu `autossh` aitavad taaskäivitamisel, kuid vajavad siiski käsitsi seadistamist ja neil puudub kasutajaliides."

uses:
  title: "Milleks see sobib"
  items:
    - id: infra
      title: "Teie oma taristu"
      text: "Pääsete ligi sisemistele juhtpaneelidele, testkeskkondadele ja haldustööriistadele, mis on kättesaadavad ainult teie serveri võrgust."
    - id: wifi
      title: "Ebausaldusväärsed võrgud"
      text: "Suunake liiklus läbi oma serveri, kui kasutate avalikku WiFi-võrku kohvikutes, hotellides või lennujaamades."
    - id: laptop
      title: "Kaugtöö sülearvutiga"
      text: "Stabiilne puhverserver arvutis, mis terve päeva magama jääb ja võrke vahetab."

how:
  title: "Valmis kolme sammuga"
  steps:
    - title: "Valmistage server ette"
      text: "Sobib iga OpenSSH-ga Linuxi server. Sellele ei paigaldata midagi — eraldi kasutaja, kes saab ainult porte edasi suunata, luuakse mõne käsuga."
    - title: "Ühendage Shprot"
      text: "Sisestage `user@server`, looge võti ja lisage selle avalik osa serveris faili `authorized_keys`. Kontrollige üks kord serveri sõrmejälge."
    - title: "Suunake rakendused puhverserverisse"
      text: "Kasutage aadressi `socks5h://localhost:1080` brauseris või süsteemi puhverserveri seadetes — teavitusala menüü kopeerib aadressi teie eest."
  guide: "Samm-sammuline juhend koos serveri turvamisega"

tour:
  title: "Lähemalt"
  items:
    - id: trust
      caption: "Esimesel ühendusel näete serveri võtme sõrmejälge ja otsustate, kas seda usaldada."
    - id: tray
      caption: "Teavitusala menüü näitab tunneli olekut ja kopeerib kasutusvalmis puhverserveri aadressid."

download:
  title: "Laadige Shprot alla"
  lead: "Versioon {version}. Tasuta, avatud lähtekoodiga, ilma kontode ja tellimusteta."
  windows:
    title: "Windows"
    text: "Windows 10 ja 11, 64-bitine. Uuendamiseks paigaldage uus versioon vana peale."
    button: "Paigaldusprogramm (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ ja ühilduvad distributsioonid."
    button: "Pakett (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ ja ühilduvad distributsioonid."
    button: "Pakett (.rpm)"
  requirements: "Shprot kasutab OpenSSH klienti (`ssh` ja `ssh-keygen`). See on olemas Windows 10 / 11-s ja enamikus Linuxi töölaudades; kui see puudub, ütleb Shprot, kuidas see lisada."
  verify: "Igas versioonis on fail `SHA256SUMS`, millega saab kontrollida, et allalaaditud faili pole muudetud."
  all_releases: "Kõik versioonid ja muudatused"

faq:
  title: "Küsimused ja vastused"
  items:
    - q: "Kas Shprot on VPN?"
      a: "Ei, see on puhverserver. Tunnelit kasutavad ainult selleks seadistatud rakendused — brauser, `curl`, `git`, IDE; kõik muu liigub otse. Virtuaalseid võrguadaptereid ei paigaldata."
    - q: "Mida on serveris vaja?"
      a: "Tavalist OpenSSH serverit ja kontot, millel on lubatud TCP-porte edasi suunata. Serverisse ei pea midagi paigaldama; sobib iga SSH kaudu kättesaadav VPS või koduserver."
    - q: "Kas saan parooliga sisse logida?"
      a: "Ei, Shprot kasutab ainult SSH-võtmeid. Võtmed on paroolidest turvalisemad ja lasevad tunnelil ilma teie sekkumiseta uuesti ühenduda. Võtme saab rakenduses luua ühe klõpsuga."
    - q: "Millised rakendused saavad puhverserverit kasutada?"
      a: "Brauserid, `curl`, `git`, paketihaldurid ja iga rakendus, millel on SOCKS5- või HTTP-puhverserveri seaded või tugi muutujatele `ALL_PROXY` / `HTTP_PROXY`. Kasutage `socks5h://`, et saitide nimed lahendataks serveris."
    - q: "Kuidas Shprot tunnelit kontrollib?"
      a: "Aeg-ajalt avab see puhverserveri kaudu juhuslikult valitud populaarse veebisaidi. Kui see ebaõnnestub, kuigi internet on kättesaadav, käivitatakse tunnel uuesti."
    - q: "Kas see on tasuta?"
      a: "Jah. Shprot on vaba ja avatud lähtekoodiga tarkvara GNU GPL v3 litsentsiga, ilma kontode, tellimuste ja reklaamideta."
    - q: "Kas macOS-i versioon on olemas?"
      a: "Veel mitte. Praegu toetab Shprot Windows 10 / 11 ja Linuxit."

footer:
  license: "Shprot on vaba tarkvara GNU GPL v3 litsentsiga."
  source: "Lähtekood"
  issues: "Teata probleemist"
---
