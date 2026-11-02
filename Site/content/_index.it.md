---
title: "Shprot — proxy SOCKS5 su SSH che si riconnette da solo"
description: "App desktop gratuita per Windows e Linux che mantiene attivo un proxy SOCKS5 e HTTP su SSH: si riconnette dopo cambi di rete e sospensione, rileva i tunnel bloccati, senza riga di comando."
readme: "README.md"

ui:
  skip: "Vai al contenuto"
  nav_features: "Funzionalità"
  nav_compare: "Perché Shprot"
  nav_download: "Download"
  nav_faq: "FAQ"
  language: "Lingua"
  github: "Codice sorgente su GitHub"

hero:
  title: "Un proxy SOCKS5 su SSH che resta connesso"
  lead: "Shprot trasforma qualsiasi server raggiungibile via SSH in un proxy SOCKS5 e HTTP stabile. Tiene d’occhio la rete, ripristina il tunnel dopo un cambio di Wi-Fi o il risveglio dalla sospensione e ti dice esattamente cosa non va, senza riga di comando."
  download: "Scarica"
  download_hint: "Versione {version} per Windows e Linux"
  facts:
    - "Gratuito e open source"
    - "Windows 10 / 11 e Linux"
    - "49 lingue"
  screenshot_alt: "Finestra delle impostazioni di Shprot"

features:
  title: "Tutto ciò che serve a un tunnel per restare attivo"
  items:
    - id: reconnect
      title: "Resiste ai cambi di rete"
      text: "Cambi Wi-Fi, colleghi un cavo o risvegli il portatile: Shprot se ne accorge e ripristina il tunnel da solo."
    - id: health
      title: "Scopre i guasti silenziosi"
      text: "Controlli periodici attraverso il proxy stesso individuano un tunnel che sembra connesso ma non fa passare traffico, e lo riavviano."
    - id: gentle
      title: "Rispetta il tuo server"
      text: "I tentativi si diradano gradualmente e, dopo accessi falliti, si ripetono al massimo una volta al minuto: fail2ban non bloccherà il tuo IP per una chiave sbagliata."
    - id: keys
      title: "Chiavi SSH in due clic"
      text: "Genera o importa una chiave e copia la parte pubblica per `authorized_keys` direttamente dalla finestra delle impostazioni."
    - id: trust
      title: "Verifica chiara del server"
      text: "Alla prima connessione l’impronta della chiave host del server viene mostrata in una finestra di dialogo, così puoi controllarla prima di fidarti del server."
    - id: http
      title: "Anche proxy HTTP"
      text: "Le app che non supportano SOCKS5 possono usare un proxy HTTP opzionale che passa per lo stesso tunnel."
    - id: tray
      title: "Vive nell’area di notifica"
      text: "L’icona mostra se il tunnel è attivo, le notifiche spiegano perché non lo è e i log sono a un clic di distanza."
    - id: languages
      title: "Parla la tua lingua"
      text: "L’interfaccia è tradotta in 49 lingue, con layout da destra a sinistra per arabo, persiano, ebraico e urdu."

compare:
  title: "Perché non usare semplicemente `ssh -D`?"
  lead: "`ssh -D 1080 user@server` ti dà un proxy SOCKS5 con un solo comando, finché la connessione non smette di funzionare senza avvisare. Shprot esegue lo stesso client OpenSSH e si occupa di tutto ciò che il comando da solo non fa."
  rows:
    - case: "Cambio di rete o di Wi-Fi, risveglio dalla sospensione"
      ssh: "Riavvialo tu"
      shprot: "Si riconnette automaticamente"
    - case: "Tunnel connesso ma senza traffico"
      ssh: "Passa inosservato"
      shprot: "Rilevato dai controlli attraverso il proxy"
    - case: "Errori di autenticazione ripetuti"
      ssh: "Affidati ai tuoi script"
      shprot: "Tentativi radi, così il server non blocca il tuo IP"
    - case: "Chiavi del client"
      ssh: "`ssh-keygen` e file in `~/.ssh`"
      shprot: "Genera o importa, copia la chiave pubblica con un clic"
    - case: "Verifica dell’identità del server"
      ssh: "Domanda nel terminale"
      shprot: "Finestra con l’impronta della chiave host"
    - case: "App senza supporto SOCKS5"
      ssh: "Non coperte"
      shprot: "Proxy HTTP opzionale nello stesso tunnel"
    - case: "Stato del tunnel"
      ssh: "Finestra del terminale"
      shprot: "Icona nell’area di notifica e notifiche con il motivo dell’errore"
  note: "Strumenti come `autossh` aiutano con i riavvii, ma richiedono comunque una configurazione manuale e non hanno un’interfaccia."

uses:
  title: "A cosa serve"
  items:
    - id: infra
      title: "La tua infrastruttura"
      text: "Raggiungi dashboard interne, ambienti di staging e strumenti di amministrazione accessibili solo dalla rete del tuo server."
    - id: wifi
      title: "Reti non affidabili"
      text: "Fai passare il traffico dal tuo server quando usi il Wi-Fi pubblico di bar, hotel o aeroporti."
    - id: laptop
      title: "Lavoro da remoto con il portatile"
      text: "Un proxy stabile su un computer che va in sospensione e cambia rete tutto il giorno."

how:
  title: "Pronto in tre passi"
  steps:
    - title: "Prepara un server"
      text: "Va bene qualsiasi server Linux con OpenSSH. Non si installa nulla: un utente dedicato che può solo inoltrare porte si crea con pochi comandi."
    - title: "Collega Shprot"
      text: "Inserisci `user@server`, genera una chiave e aggiungi la parte pubblica a `authorized_keys` sul server. Verifica una volta l’impronta del server."
    - title: "Indirizza le app al proxy"
      text: "Usa `socks5h://localhost:1080` nel browser o nelle impostazioni proxy di sistema: il menu dell’icona copia l’indirizzo per te."
  guide: "Guida passo passo con protezione del server"

tour:
  title: "Più da vicino"
  items:
    - id: trust
      caption: "Alla prima connessione vedi l’impronta della chiave del server e decidi se fidarti."
    - id: tray
      caption: "Il menu dell’icona mostra lo stato del tunnel e copia indirizzi proxy pronti all’uso."

download:
  title: "Scarica Shprot"
  lead: "Versione {version}. Gratuito, open source, senza account né abbonamenti."
  windows:
    title: "Windows"
    text: "Windows 10 e 11, 64 bit. Per aggiornare, installa la nuova versione sopra quella vecchia."
    button: "Programma di installazione (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ e distribuzioni compatibili."
    button: "Pacchetto (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ e distribuzioni compatibili."
    button: "Pacchetto (.rpm)"
  requirements: "Shprot usa il client OpenSSH (`ssh` e `ssh-keygen`). È incluso in Windows 10 / 11 e nella maggior parte dei desktop Linux; se manca, Shprot ti spiega come aggiungerlo."
  verify: "Ogni versione include un file `SHA256SUMS` per verificare che il download non sia stato alterato."
  all_releases: "Tutte le versioni e le modifiche"

faq:
  title: "Domande e risposte"
  items:
    - q: "Shprot è una VPN?"
      a: "No, è un proxy. Solo le app configurate per usarlo (un browser, `curl`, `git`, un IDE) mandano traffico nel tunnel; tutto il resto va diretto. Non vengono installate schede di rete virtuali."
    - q: "Cosa serve sul server?"
      a: "Un normale server OpenSSH e un account autorizzato a inoltrare porte TCP. Sul server non va installato nulla; va bene qualsiasi VPS o server di casa raggiungibile via SSH."
    - q: "Posso accedere con una password?"
      a: "No, Shprot usa solo chiavi SSH. Le chiavi sono più sicure delle password e permettono al tunnel di riconnettersi senza intervento. La chiave si genera nell’app con un clic."
    - q: "Quali app possono usare il proxy?"
      a: "Browser, `curl`, `git`, gestori di pacchetti e qualsiasi app con impostazioni proxy SOCKS5 o HTTP o che supporti le variabili `ALL_PROXY` / `HTTP_PROXY`. Usa `socks5h://` perché i nomi dei siti vengano risolti sul server."
    - q: "Come controlla il tunnel Shprot?"
      a: "Di tanto in tanto apre attraverso il proxy un sito popolare scelto a caso. Se non ci riesce mentre internet è disponibile, il tunnel viene riavviato."
    - q: "È gratuito?"
      a: "Sì. Shprot è software libero e open source con licenza GNU GPL v3, senza account, abbonamenti né pubblicità."
    - q: "Esiste una versione per macOS?"
      a: "Non ancora. Per ora Shprot supporta Windows 10 / 11 e Linux."

footer:
  license: "Shprot è software libero con licenza GNU GPL v3."
  source: "Codice sorgente"
  issues: "Segnala un problema"
---
