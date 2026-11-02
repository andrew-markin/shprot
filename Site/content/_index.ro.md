---
title: "Shprot — proxy SOCKS5 prin SSH care se reconectează singur"
description: "Aplicație desktop gratuită pentru Windows și Linux care menține activ un proxy SOCKS5 și HTTP prin SSH: se reconectează după schimbarea rețelei și ieșirea din repaus, detectează tunelurile blocate, fără linie de comandă."
readme: "README.md"

ui:
  skip: "Salt la conținut"
  nav_features: "Funcții"
  nav_compare: "De ce Shprot"
  nav_download: "Descărcare"
  nav_faq: "Întrebări"
  language: "Limbă"
  github: "Codul sursă pe GitHub"

hero:
  title: "Un proxy SOCKS5 prin SSH care rămâne conectat"
  lead: "Shprot transformă orice server accesibil prin SSH într-un proxy SOCKS5 și HTTP stabil. Urmărește rețeaua, readuce tunelul după schimbarea rețelei Wi-Fi sau ieșirea din repaus și vă spune exact ce nu a mers — fără linie de comandă."
  download: "Descărcați"
  download_hint: "Versiunea {version} pentru Windows și Linux"
  facts:
    - "Gratuit și open source"
    - "Windows 10 / 11 și Linux"
    - "49 de limbi"
  screenshot_alt: "Fereastra de setări Shprot"

features:
  title: "Tot ce îi trebuie unui tunel ca să funcționeze în continuare"
  items:
    - id: reconnect
      title: "Rezistă la schimbarea rețelei"
      text: "Schimbați rețeaua Wi-Fi, conectați un cablu sau treziți laptopul din repaus — Shprot observă și readuce singur tunelul."
    - id: health
      title: "Prinde defecțiunile tăcute"
      text: "Verificări regulate prin proxy-ul însuși găsesc un tunel care pare conectat, dar nu lasă traficul să treacă, și îl repornesc."
    - id: gentle
      title: "Blând cu serverul dvs."
      text: "Reîncercările se răresc treptat, iar după autentificări eșuate se repetă cel mult o dată pe minut, așa că fail2ban nu vă va bloca IP-ul din cauza unei chei greșite."
    - id: keys
      title: "Chei SSH din două clicuri"
      text: "Generați sau importați o cheie și copiați partea publică pentru `authorized_keys` direct din fereastra de setări."
    - id: trust
      title: "Verificare clară a serverului"
      text: "La prima conexiune, amprenta cheii serverului este afișată într-un dialog, ca să o puteți verifica înainte de a avea încredere în server."
    - id: http
      title: "Și proxy HTTP"
      text: "Aplicațiile fără suport SOCKS5 pot folosi un proxy HTTP opțional care trece prin același tunel."
    - id: tray
      title: "Stă în zona de notificare"
      text: "Pictograma arată dacă tunelul este activ, notificările explică de ce nu este, iar jurnalele sunt la un clic distanță."
    - id: languages
      title: "Vorbește limba dvs."
      text: "Interfața este tradusă în 49 de limbi, cu aranjare de la dreapta la stânga pentru arabă, persană, ebraică și urdu."

compare:
  title: "De ce nu simplu `ssh -D`?"
  lead: "`ssh -D 1080 user@server` vă oferă un proxy SOCKS5 dintr-o singură comandă — până când conexiunea încetează să funcționeze fără niciun semn. Shprot rulează același client OpenSSH și se ocupă de tot ce comanda singură nu face."
  rows:
    - case: "Schimbarea rețelei sau a rețelei Wi-Fi, ieșirea din repaus"
      ssh: "Reporniți-l manual"
      shprot: "Se reconectează automat"
    - case: "Tunel conectat, dar fără trafic"
      ssh: "Trece neobservat"
      shprot: "Detectat prin verificări prin proxy"
    - case: "Erori repetate de autentificare"
      ssh: "Rămâne în seama scripturilor dvs."
      shprot: "Reîncercări rare, ca serverul să nu vă blocheze IP-ul"
    - case: "Chei client"
      ssh: "`ssh-keygen` și fișiere în `~/.ssh`"
      shprot: "Generare sau import, copierea cheii publice dintr-un clic"
    - case: "Verificarea identității serverului"
      ssh: "Întrebare în terminal"
      shprot: "Dialog cu amprenta cheii serverului"
    - case: "Aplicații fără suport SOCKS5"
      ssh: "Nu sunt acoperite"
      shprot: "Proxy HTTP opțional prin același tunel"
    - case: "Starea tunelului"
      ssh: "Fereastra terminalului"
      shprot: "Pictogramă în zona de notificare și notificări cu motivul erorii"
  note: "Instrumente precum `autossh` ajută la repornire, dar tot necesită configurare manuală și nu au interfață."

uses:
  title: "La ce folosește"
  items:
    - id: infra
      title: "Propria infrastructură"
      text: "Accesați panouri interne, medii de testare și instrumente de administrare disponibile doar din rețeaua serverului dvs."
    - id: wifi
      title: "Rețele nesigure"
      text: "Trimiteți traficul prin propriul server când folosiți Wi-Fi public în cafenele, hoteluri sau aeroporturi."
    - id: laptop
      title: "Lucru la distanță pe laptop"
      text: "Un proxy stabil pe un calculator care intră în repaus și schimbă rețeaua toată ziua."

how:
  title: "Gata în trei pași"
  steps:
    - title: "Pregătiți un server"
      text: "Orice server Linux cu OpenSSH este potrivit. Nu se instalează nimic pe el: un utilizator dedicat, care poate doar redirecționa porturi, se creează cu câteva comenzi."
    - title: "Conectați Shprot"
      text: "Introduceți `user@server`, generați o cheie și adăugați partea publică în `authorized_keys` pe server. Verificați o singură dată amprenta serverului."
    - title: "Direcționați aplicațiile spre proxy"
      text: "Folosiți `socks5h://localhost:1080` în browser sau în setările proxy ale sistemului — meniul din zona de notificare copiază adresa pentru dvs."
  guide: "Ghid pas cu pas cu securizarea serverului"

tour:
  title: "Mai de aproape"
  items:
    - id: trust
      caption: "La prima conexiune vedeți amprenta cheii serverului și decideți dacă aveți încredere în el."
    - id: tray
      caption: "Meniul din zona de notificare arată starea tunelului și copiază adrese proxy gata de folosit."

download:
  title: "Descărcați Shprot"
  lead: "Versiunea {version}. Gratuit, open source, fără conturi sau abonamente."
  windows:
    title: "Windows"
    text: "Windows 10 și 11, 64 de biți. Pentru actualizare, instalați versiunea nouă peste cea veche."
    button: "Program de instalare (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ și distribuții compatibile."
    button: "Pachet (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ și distribuții compatibile."
    button: "Pachet (.rpm)"
  requirements: "Shprot folosește clientul OpenSSH (`ssh` și `ssh-keygen`). Acesta este inclus în Windows 10 / 11 și în majoritatea mediilor desktop Linux; dacă lipsește, Shprot vă spune cum să-l adăugați."
  verify: "Fiecare versiune include un fișier `SHA256SUMS` pentru a verifica dacă fișierul descărcat nu a fost modificat."
  all_releases: "Toate versiunile și modificările"

faq:
  title: "Întrebări și răspunsuri"
  items:
    - q: "Shprot este un VPN?"
      a: "Nu, este un proxy. Doar aplicațiile configurate să îl folosească — un browser, `curl`, `git`, un IDE — trimit trafic prin tunel; restul merge direct. Nu se instalează adaptoare de rețea virtuale."
    - q: "De ce am nevoie pe server?"
      a: "De un server OpenSSH obișnuit și de un cont cu permisiunea de a redirecționa porturi TCP. Nu trebuie instalat nimic pe server; orice VPS sau server de acasă accesibil prin SSH este potrivit."
    - q: "Mă pot autentifica cu o parolă?"
      a: "Nu, Shprot folosește doar chei SSH. Cheile sunt mai sigure decât parolele și permit tunelului să se reconecteze fără intervenția dvs. Cheia se generează în aplicație dintr-un clic."
    - q: "Ce aplicații pot folosi proxy-ul?"
      a: "Browsere, `curl`, `git`, manageri de pachete și orice aplicație cu setări de proxy SOCKS5 sau HTTP ori cu suport pentru variabilele `ALL_PROXY` / `HTTP_PROXY`. Folosiți `socks5h://` pentru ca numele site-urilor să fie rezolvate pe server."
    - q: "Cum verifică Shprot tunelul?"
      a: "Din când în când deschide prin proxy un site popular ales la întâmplare. Dacă nu reușește deși internetul este disponibil, tunelul este repornit."
    - q: "Este gratuit?"
      a: "Da. Shprot este software liber și open source sub licența GNU GPL v3, fără conturi, abonamente sau reclame."
    - q: "Există o versiune pentru macOS?"
      a: "Încă nu. Momentan Shprot funcționează pe Windows 10 / 11 și Linux."

footer:
  license: "Shprot este software liber sub licența GNU GPL v3."
  source: "Codul sursă"
  issues: "Raportați o problemă"
---
