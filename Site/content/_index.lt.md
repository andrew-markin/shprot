---
title: "Shprot — SOCKS5 tarpinis serveris per SSH, kuris pats prisijungia iš naujo"
description: "Nemokama programa, skirta Windows ir Linux, palaikanti veikiantį SOCKS5 ir HTTP tarpinį serverį per SSH: prisijungia iš naujo pakeitus tinklą ir po miego, aptinka užstrigusius tunelius, be komandų eilutės."
readme: "README.md"

ui:
  skip: "Pereiti prie turinio"
  nav_features: "Galimybės"
  nav_compare: "Kodėl Shprot"
  nav_download: "Atsisiųsti"
  nav_faq: "DUK"
  language: "Kalba"
  github: "Išeitinis kodas „GitHub“"

hero:
  title: "SOCKS5 tarpinis serveris per SSH, kuris lieka prisijungęs"
  lead: "Shprot bet kurį serverį, pasiekiamą per SSH, paverčia stabiliu SOCKS5 ir HTTP tarpiniu serveriu. Jis stebi tinklą, atkuria tunelį pakeitus „Wi-Fi“ ar pabudus iš miego ir tiksliai pasako, kas nutiko — be komandų eilutės."
  download: "Atsisiųsti"
  download_hint: "{version} versija, skirta Windows ir Linux"
  facts:
    - "Nemokama ir atvirojo kodo"
    - "Windows 10 / 11 ir Linux"
    - "49 kalbos"
  screenshot_alt: "Shprot nustatymų langas"

features:
  title: "Viskas, ko reikia, kad tunelis veiktų toliau"
  items:
    - id: reconnect
      title: "Atlaiko tinklo pakeitimus"
      text: "Pakeitėte „Wi-Fi“, prijungėte laidą ar pažadinote nešiojamąjį kompiuterį — Shprot tai pastebi ir pats atkuria tunelį."
    - id: health
      title: "Pagauna tylius gedimus"
      text: "Reguliarūs patikrinimai per patį tarpinį serverį randa tunelį, kuris atrodo prijungtas, bet nepraleidžia srauto, ir jį paleidžia iš naujo."
    - id: gentle
      title: "Tausoja jūsų serverį"
      text: "Pakartotiniai bandymai palaipsniui retėja, o po nesėkmingų prisijungimų kartojami ne dažniau kaip kartą per minutę, todėl fail2ban neužblokuos jūsų IP dėl neteisingo rakto."
    - id: keys
      title: "SSH raktai dviem paspaudimais"
      text: "Sukurkite arba importuokite raktą ir nukopijuokite viešąją dalį į `authorized_keys` tiesiai iš nustatymų lango."
    - id: trust
      title: "Aiškus serverio patikrinimas"
      text: "Pirmą kartą jungiantis serverio rakto piršto atspaudas parodomas dialogo lange, kad galėtumėte jį patikrinti prieš pasitikėdami serveriu."
    - id: http
      title: "Ir HTTP tarpinis serveris"
      text: "Programos be SOCKS5 palaikymo gali naudoti papildomą HTTP tarpinį serverį, veikiantį per tą patį tunelį."
    - id: tray
      title: "Gyvena sistemos dėkle"
      text: "Piktograma rodo, ar tunelis veikia, pranešimai paaiškina, kodėl ne, o žurnalas pasiekiamas vienu paspaudimu."
    - id: languages
      title: "Kalba jūsų kalba"
      text: "Sąsaja išversta į 49 kalbas, arabų, persų, hebrajų ir urdu kalboms naudojamas išdėstymas iš dešinės į kairę."

compare:
  title: "Kodėl ne tiesiog `ssh -D`?"
  lead: "`ssh -D 1080 user@server` viena komanda suteikia SOCKS5 tarpinį serverį — kol ryšys nepastebimai nustoja veikti. Shprot paleidžia tą patį OpenSSH klientą ir pasirūpina viskuo, ko viena komanda nedaro."
  rows:
    - case: "Tinklo ar „Wi-Fi“ pakeitimas, pabudimas iš miego"
      ssh: "Paleiskite iš naujo patys"
      shprot: "Prisijungia iš naujo automatiškai"
    - case: "Tunelis prijungtas, bet srautas neina"
      ssh: "Lieka nepastebėta"
      shprot: "Aptinkama patikrinimais per tarpinį serverį"
    - case: "Pasikartojančios autentifikavimo klaidos"
      ssh: "Priklauso nuo jūsų scenarijų"
      shprot: "Reti bandymai, kad serveris neužblokuotų jūsų IP"
    - case: "Kliento raktai"
      ssh: "`ssh-keygen` ir failai aplanke `~/.ssh`"
      shprot: "Sukurkite ar importuokite, viešąjį raktą nukopijuokite vienu paspaudimu"
    - case: "Serverio tapatybės patikrinimas"
      ssh: "Klausimas terminale"
      shprot: "Dialogo langas su rakto piršto atspaudu"
    - case: "Programos be SOCKS5 palaikymo"
      ssh: "Nepalaikomos"
      shprot: "Papildomas HTTP tarpinis serveris per tą patį tunelį"
    - case: "Tunelio būsena"
      ssh: "Terminalo langas"
      shprot: "Piktograma ir pranešimai su gedimo priežastimi"
  note: "Tokie įrankiai kaip `autossh` padeda paleisti iš naujo, bet vis tiek reikalauja rankinio konfigūravimo ir neturi sąsajos."

uses:
  title: "Kam jis tinka"
  items:
    - id: infra
      title: "Jūsų infrastruktūra"
      text: "Pasiekite vidinius skydelius, testavimo aplinkas ir administravimo įrankius, prieinamus tik iš jūsų serverio tinklo."
    - id: wifi
      title: "Nepatikimi tinklai"
      text: "Siųskite srautą per savo serverį, kai naudojatės viešuoju „Wi-Fi“ kavinėse, viešbučiuose ar oro uostuose."
    - id: laptop
      title: "Nuotolinis darbas nešiojamuoju kompiuteriu"
      text: "Stabilus tarpinis serveris kompiuteryje, kuris visą dieną užmiega ir keičia tinklus."

how:
  title: "Paruošta per tris žingsnius"
  steps:
    - title: "Paruoškite serverį"
      text: "Tinka bet kuris Linux serveris su OpenSSH. Į jį nieko nediegiama — atskiras naudotojas, kuriam leidžiama tik peradresuoti prievadus, sukuriamas keliomis komandomis."
    - title: "Prijunkite Shprot"
      text: "Įveskite `user@server`, sukurkite raktą ir jo viešąją dalį įtraukite į serverio `authorized_keys`. Vieną kartą patikrinkite serverio piršto atspaudą."
    - title: "Nukreipkite programas į tarpinį serverį"
      text: "Naudokite `socks5h://localhost:1080` naršyklėje arba sistemos tarpinio serverio nustatymuose — dėklo meniu nukopijuos adresą už jus."
  guide: "Nuoseklus vadovas su serverio apsauga"

tour:
  title: "Iš arčiau"
  items:
    - id: trust
      caption: "Pirmą kartą jungdamiesi matote serverio rakto piršto atspaudą ir sprendžiate, ar juo pasitikėti."
    - id: tray
      caption: "Dėklo meniu rodo tunelio būseną ir kopijuoja paruoštus tarpinio serverio adresus."

download:
  title: "Atsisiųskite Shprot"
  lead: "{version} versija. Nemokama, atvirojo kodo, be paskyrų ir prenumeratų."
  windows:
    title: "Windows"
    text: "Windows 10 ir 11, 64 bitų. Norėdami atnaujinti, įdiekite naują versiją ant senosios."
    button: "Diegimo programa (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ ir suderinami platinimai."
    button: "Paketas (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ ir suderinami platinimai."
    button: "Paketas (.rpm)"
  requirements: "Shprot naudoja OpenSSH klientą (`ssh` ir `ssh-keygen`). Jis yra Windows 10 / 11 ir daugumoje Linux darbalaukių; jei jo nėra, Shprot pasakys, kaip jį pridėti."
  verify: "Kiekvienoje versijoje yra failas `SHA256SUMS`, kuriuo galite patikrinti, ar atsisiųstas failas nepakeistas."
  all_releases: "Visos versijos ir pakeitimai"

faq:
  title: "Klausimai ir atsakymai"
  items:
    - q: "Ar Shprot yra VPN?"
      a: "Ne, tai tarpinis serveris. Per tunelį srautą siunčia tik jam naudoti nustatytos programos — naršyklė, `curl`, `git`, IDE; visa kita eina tiesiogiai. Virtualūs tinklo adapteriai nediegiami."
    - q: "Ko reikia serveryje?"
      a: "Įprasto OpenSSH serverio ir paskyros, kuriai leidžiama peradresuoti TCP prievadus. Į serverį nieko diegti nereikia; tinka bet kuris per SSH pasiekiamas VPS ar namų serveris."
    - q: "Ar galiu jungtis slaptažodžiu?"
      a: "Ne, Shprot naudoja tik SSH raktus. Raktai saugesni už slaptažodžius ir leidžia tuneliui prisijungti iš naujo be jūsų įsikišimo. Raktas programoje sukuriamas vienu paspaudimu."
    - q: "Kurios programos gali naudoti tarpinį serverį?"
      a: "Naršyklės, `curl`, `git`, paketų tvarkytuvės ir bet kuri programa su SOCKS5 ar HTTP tarpinio serverio nustatymais arba palaikanti kintamuosius `ALL_PROXY` / `HTTP_PROXY`. Naudokite `socks5h://`, kad svetainių vardai būtų nustatomi serveryje."
    - q: "Kaip Shprot tikrina tunelį?"
      a: "Retkarčiais per tarpinį serverį jis atidaro atsitiktinai pasirinktą populiarią svetainę. Jei nepavyksta, nors internetas pasiekiamas, tunelis paleidžiamas iš naujo."
    - q: "Ar tai nemokama?"
      a: "Taip. Shprot — laisva atvirojo kodo programinė įranga pagal GNU GPL v3 licenciją, be paskyrų, prenumeratų ir reklamos."
    - q: "Ar yra macOS versija?"
      a: "Kol kas ne. Šiuo metu Shprot palaiko Windows 10 / 11 ir Linux."

footer:
  license: "Shprot — laisva programinė įranga pagal GNU GPL v3 licenciją."
  source: "Išeitinis kodas"
  issues: "Pranešti apie problemą"
---
