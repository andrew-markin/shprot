---
title: "Shprot — servidor intermediari SOCKS5 per SSH que es reconnecta sol"
description: "Aplicació d’escriptori gratuïta per a Windows i Linux que manté actiu un servidor intermediari SOCKS5 i HTTP per SSH: es reconnecta després de canvis de xarxa i suspensió, detecta túnels encallats, sense línia d’ordres."
readme: "README.md"

ui:
  skip: "Ves al contingut"
  nav_features: "Funcions"
  nav_compare: "Per què Shprot"
  nav_download: "Baixa"
  nav_faq: "Preguntes"
  language: "Llengua"
  github: "Codi font a GitHub"

hero:
  title: "Un servidor intermediari SOCKS5 per SSH que no es desconnecta"
  lead: "Shprot converteix qualsevol servidor al qual accediu per SSH en un servidor intermediari SOCKS5 i HTTP estable. Vigila la xarxa, recupera el túnel després de canviar de Wi-Fi o de sortir de la suspensió i us diu exactament què ha fallat, sense línia d’ordres."
  download: "Baixa"
  download_hint: "Versió {version} per a Windows i Linux"
  facts:
    - "Gratuït i de codi obert"
    - "Windows 10 / 11 i Linux"
    - "49 llengües"
  screenshot_alt: "Finestra de configuració de Shprot"

features:
  title: "Tot el que un túnel necessita per continuar funcionant"
  items:
    - id: reconnect
      title: "Suporta els canvis de xarxa"
      text: "Canvieu de Wi-Fi, connecteu un cable o desperteu el portàtil: Shprot ho detecta i recupera el túnel tot sol."
    - id: health
      title: "Detecta errors silenciosos"
      text: "Unes comprovacions periòdiques a través del mateix servidor intermediari troben un túnel que sembla connectat però no deixa passar trànsit, i el reinicien."
    - id: gentle
      title: "Respecta el vostre servidor"
      text: "Els reintents s’espaien a poc a poc i, després d’inicis de sessió fallits, es repeteixen com a màxim un cop per minut, de manera que fail2ban no bloquejarà la vostra IP per una clau incorrecta."
    - id: keys
      title: "Claus SSH en dos clics"
      text: "Genereu o importeu una clau i copieu-ne la part pública per a `authorized_keys` directament des de la finestra de configuració."
    - id: trust
      title: "Verificació clara del servidor"
      text: "En la primera connexió, l’empremta de la clau del servidor es mostra en un diàleg perquè la pugueu comprovar abans de confiar-hi."
    - id: http
      title: "També servidor intermediari HTTP"
      text: "Les aplicacions sense suport per a SOCKS5 poden fer servir un servidor intermediari HTTP opcional que passa pel mateix túnel."
    - id: tray
      title: "Viu a la safata del sistema"
      text: "La icona mostra si el túnel està actiu, les notificacions expliquen per què no ho està i els registres són a un clic."
    - id: languages
      title: "Parla la vostra llengua"
      text: "La interfície està traduïda a 49 llengües, amb disposició de dreta a esquerra per a l’àrab, el persa, l’hebreu i l’urdú."

compare:
  title: "Per què no simplement `ssh -D`?"
  lead: "`ssh -D 1080 user@server` us dona un servidor intermediari SOCKS5 amb una sola ordre, fins que la connexió deixa de funcionar sense avisar. Shprot executa el mateix client OpenSSH i s’encarrega de tot el que l’ordre sola no fa."
  rows:
    - case: "Canvi de xarxa o de Wi-Fi, sortida de la suspensió"
      ssh: "Reinicieu-lo vosaltres"
      shprot: "Es reconnecta automàticament"
    - case: "Túnel connectat però sense trànsit"
      ssh: "Passa desapercebut"
      shprot: "El detecten les comprovacions a través del servidor intermediari"
    - case: "Errors d’autenticació repetits"
      ssh: "Depèn dels vostres scripts"
      shprot: "Reintents espaiats perquè el servidor no bloquegi la vostra IP"
    - case: "Claus del client"
      ssh: "`ssh-keygen` i fitxers a `~/.ssh`"
      shprot: "Generar o importar, copiar la clau pública amb un clic"
    - case: "Verificació de la identitat del servidor"
      ssh: "Pregunta al terminal"
      shprot: "Diàleg amb l’empremta de la clau del servidor"
    - case: "Aplicacions sense suport per a SOCKS5"
      ssh: "Sense solució"
      shprot: "Servidor intermediari HTTP opcional pel mateix túnel"
    - case: "Estat del túnel"
      ssh: "Finestra del terminal"
      shprot: "Icona a la safata i notificacions amb el motiu de l’error"
  note: "Eines com `autossh` ajuden amb els reinicis, però continuen necessitant configuració manual i no tenen interfície."

uses:
  title: "Per a què serveix"
  items:
    - id: infra
      title: "La vostra infraestructura"
      text: "Accediu a taulers interns, entorns de proves i eines d’administració que només són accessibles des de la xarxa del vostre servidor."
    - id: wifi
      title: "Xarxes poc fiables"
      text: "Envieu el trànsit pel vostre propi servidor quan feu servir Wi-Fi públic en cafeteries, hotels o aeroports."
    - id: laptop
      title: "Teletreball amb portàtil"
      text: "Un servidor intermediari estable en un ordinador que se suspèn i canvia de xarxa tot el dia."

how:
  title: "A punt en tres passos"
  steps:
    - title: "Prepareu un servidor"
      text: "Serveix qualsevol servidor Linux amb OpenSSH. No cal instal·lar-hi res: un usuari dedicat que només pot reenviar ports es crea amb unes poques ordres."
    - title: "Connecteu Shprot"
      text: "Introduïu `user@server`, genereu una clau i afegiu-ne la part pública a `authorized_keys` al servidor. Comproveu una vegada l’empremta del servidor."
    - title: "Dirigiu les aplicacions al servidor intermediari"
      text: "Feu servir `socks5h://localhost:1080` al navegador o a la configuració del sistema; el menú de la safata us copia l’adreça."
  guide: "Guia pas a pas amb protecció del servidor"

tour:
  title: "Més de prop"
  items:
    - id: trust
      caption: "En la primera connexió veieu l’empremta de la clau del servidor i decidiu si hi confieu."
    - id: tray
      caption: "El menú de la safata mostra l’estat del túnel i copia adreces a punt per fer servir."

download:
  title: "Baixeu Shprot"
  lead: "Versió {version}. Gratuït, de codi obert, sense comptes ni subscripcions."
  windows:
    title: "Windows"
    text: "Windows 10 i 11, 64 bits. Per actualitzar, instal·leu la versió nova damunt de l’antiga."
    button: "Instal·lador (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ i distribucions compatibles."
    button: "Paquet (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ i distribucions compatibles."
    button: "Paquet (.rpm)"
  requirements: "Shprot fa servir el client OpenSSH (`ssh` i `ssh-keygen`). Ve inclòs a Windows 10 / 11 i a la majoria d’escriptoris Linux; si no hi és, Shprot us explicarà com afegir-lo."
  verify: "Cada versió inclou un fitxer `SHA256SUMS` per comprovar que la baixada no s’ha alterat."
  all_releases: "Totes les versions i canvis"

faq:
  title: "Preguntes i respostes"
  items:
    - q: "Shprot és una VPN?"
      a: "No, és un servidor intermediari. Només les aplicacions configurades per fer-lo servir (un navegador, `curl`, `git`, un IDE) envien trànsit pel túnel; la resta va directament. No s’instal·len adaptadors de xarxa virtuals."
    - q: "Què necessito al servidor?"
      a: "Un servidor OpenSSH normal i un compte amb permís per reenviar ports TCP. No cal instal·lar res al servidor; serveix qualsevol VPS o servidor domèstic accessible per SSH."
    - q: "Puc iniciar sessió amb contrasenya?"
      a: "No, Shprot només fa servir claus SSH. Les claus són més segures que les contrasenyes i permeten que el túnel es reconnecti sense intervenció. La clau es genera a l’aplicació amb un clic."
    - q: "Quines aplicacions poden fer servir el servidor intermediari?"
      a: "Navegadors, `curl`, `git`, gestors de paquets i qualsevol aplicació amb configuració de servidor intermediari SOCKS5 o HTTP o compatible amb les variables `ALL_PROXY` / `HTTP_PROXY`. Feu servir `socks5h://` perquè els noms dels llocs es resolguin al servidor."
    - q: "Com comprova Shprot el túnel?"
      a: "De tant en tant obre a través del servidor intermediari un lloc web popular triat a l’atzar. Si falla mentre internet està disponible, el túnel es reinicia."
    - q: "És gratuït?"
      a: "Sí. Shprot és programari lliure i de codi obert amb llicència GNU GPL v3, sense comptes, subscripcions ni anuncis."
    - q: "Hi ha versió per a macOS?"
      a: "Encara no. Ara com ara Shprot és compatible amb Windows 10 / 11 i Linux."

footer:
  license: "Shprot és programari lliure amb llicència GNU GPL v3."
  source: "Codi font"
  issues: "Informeu d’un problema"
---
