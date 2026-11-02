---
title: "Shprot — proxy SOCKS5 via SSH qui se reconnecte tout seul"
description: "Application de bureau gratuite pour Windows et Linux qui maintient un proxy SOCKS5 et HTTP via SSH : reconnexion après un changement de réseau ou une mise en veille, détection des tunnels bloqués, sans ligne de commande."
readme: "README.md"

ui:
  skip: "Aller au contenu"
  nav_features: "Fonctionnalités"
  nav_compare: "Pourquoi Shprot"
  nav_download: "Télécharger"
  nav_faq: "FAQ"
  language: "Langue"
  github: "Code source sur GitHub"

hero:
  title: "Un proxy SOCKS5 via SSH qui reste connecté"
  lead: "Shprot transforme n’importe quel serveur accessible en SSH en proxy SOCKS5 et HTTP stable. Il surveille votre réseau, rétablit le tunnel après un changement de Wi-Fi ou une sortie de veille et vous dit précisément ce qui ne va pas — sans ligne de commande."
  download: "Télécharger"
  download_hint: "Version {version} pour Windows et Linux"
  facts:
    - "Gratuit et open source"
    - "Windows 10 / 11 et Linux"
    - "49 langues"
  screenshot_alt: "Fenêtre des paramètres de Shprot"

features:
  title: "Tout ce qu’il faut pour qu’un tunnel tienne"
  items:
    - id: reconnect
      title: "Survit aux changements de réseau"
      text: "Changement de Wi-Fi, câble branché ou portable sorti de veille : Shprot le remarque et rétablit le tunnel de lui-même."
    - id: health
      title: "Repère les pannes silencieuses"
      text: "Des vérifications régulières à travers le proxy lui-même détectent un tunnel qui semble connecté mais ne laisse rien passer, et le redémarrent."
    - id: gentle
      title: "Ménage votre serveur"
      text: "Les tentatives s’espacent progressivement et, après un échec d’authentification, elles ont lieu au plus une fois par minute : fail2ban ne bannira pas votre IP à cause d’une mauvaise clé."
    - id: keys
      title: "Clés SSH en deux clics"
      text: "Générez ou importez une clé et copiez sa partie publique pour `authorized_keys` directement depuis la fenêtre des paramètres."
    - id: trust
      title: "Vérification claire du serveur"
      text: "À la première connexion, l’empreinte de la clé d’hôte du serveur s’affiche dans une boîte de dialogue pour que vous puissiez la vérifier avant de lui faire confiance."
    - id: http
      title: "Un proxy HTTP en plus"
      text: "Les applications qui ne prennent pas en charge SOCKS5 peuvent utiliser un proxy HTTP optionnel qui passe par le même tunnel."
    - id: tray
      title: "Dans la zone de notification"
      text: "L’icône indique si le tunnel est actif, les notifications expliquent pourquoi il ne l’est pas, et les journaux sont à un clic."
    - id: languages
      title: "Parle votre langue"
      text: "L’interface est traduite en 49 langues, avec une mise en page de droite à gauche pour l’arabe, le persan, l’hébreu et l’ourdou."

compare:
  title: "Pourquoi pas simplement `ssh -D` ?"
  lead: "`ssh -D 1080 user@server` vous donne un proxy SOCKS5 en une commande — jusqu’à ce que la connexion cesse de fonctionner sans prévenir. Shprot lance le même client OpenSSH et s’occupe de tout ce que la commande seule ne fait pas."
  rows:
    - case: "Changement de réseau ou de Wi-Fi, sortie de veille"
      ssh: "À relancer vous-même"
      shprot: "Reconnexion automatique"
    - case: "Tunnel connecté mais sans trafic"
      ssh: "Passe inaperçu"
      shprot: "Détecté par des vérifications à travers le proxy"
    - case: "Échecs d’authentification répétés"
      ssh: "À la charge de vos scripts"
      shprot: "Tentatives espacées pour que le serveur ne bannisse pas votre IP"
    - case: "Clés client"
      ssh: "`ssh-keygen` et fichiers dans `~/.ssh`"
      shprot: "Générer ou importer, copier la clé publique en un clic"
    - case: "Vérification de l’identité du serveur"
      ssh: "Question dans le terminal"
      shprot: "Boîte de dialogue avec l’empreinte de la clé d’hôte"
    - case: "Applications sans prise en charge de SOCKS5"
      ssh: "Non couvert"
      shprot: "Proxy HTTP optionnel par le même tunnel"
    - case: "État du tunnel"
      ssh: "Fenêtre de terminal"
      shprot: "Icône et notifications indiquant la cause de l’échec"
  note: "Des outils comme `autossh` aident pour les redémarrages, mais demandent toujours une configuration manuelle et n’ont pas d’interface."

uses:
  title: "À quoi ça sert"
  items:
    - id: infra
      title: "Votre propre infrastructure"
      text: "Accédez aux tableaux de bord internes, environnements de préproduction et outils d’administration accessibles uniquement depuis le réseau de votre serveur."
    - id: wifi
      title: "Réseaux non fiables"
      text: "Faites passer votre trafic par votre propre serveur sur le Wi-Fi public des cafés, hôtels ou aéroports."
    - id: laptop
      title: "Télétravail sur portable"
      text: "Gardez un proxy stable sur une machine qui se met en veille et change de réseau toute la journée."

how:
  title: "Opérationnel en trois étapes"
  steps:
    - title: "Préparez un serveur"
      text: "N’importe quel serveur Linux avec OpenSSH convient. Rien n’y est installé : un utilisateur dédié, limité au transfert de ports, se crée en quelques commandes."
    - title: "Connectez Shprot"
      text: "Saisissez `user@server`, générez une clé et ajoutez sa partie publique au fichier `authorized_keys` du serveur. Vérifiez une fois l’empreinte du serveur."
    - title: "Orientez les applications vers le proxy"
      text: "Indiquez `socks5h://localhost:1080` dans le navigateur ou les paramètres proxy du système — le menu de l’icône copie l’adresse pour vous."
  guide: "Guide pas à pas avec sécurisation du serveur"

tour:
  title: "De plus près"
  items:
    - id: trust
      caption: "À la première connexion, vous voyez l’empreinte de la clé du serveur et décidez si vous lui faites confiance."
    - id: tray
      caption: "Le menu de l’icône affiche l’état du tunnel et copie des adresses de proxy prêtes à l’emploi."

download:
  title: "Télécharger Shprot"
  lead: "Version {version}. Gratuit, open source, sans compte ni abonnement."
  windows:
    title: "Windows"
    text: "Windows 10 et 11, 64 bits. Pour mettre à jour, installez la nouvelle version par-dessus l’ancienne."
    button: "Programme d’installation (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ et distributions compatibles."
    button: "Paquet (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ et distributions compatibles."
    button: "Paquet (.rpm)"
  requirements: "Shprot utilise le client OpenSSH (`ssh` et `ssh-keygen`). Il est fourni avec Windows 10 / 11 et la plupart des bureaux Linux ; s’il manque, Shprot vous indique comment l’ajouter."
  verify: "Chaque version contient un fichier `SHA256SUMS` pour vérifier que le téléchargement n’a pas été modifié."
  all_releases: "Toutes les versions et modifications"

faq:
  title: "Questions et réponses"
  items:
    - q: "Shprot est-il un VPN ?"
      a: "Non, c’est un proxy. Seules les applications configurées pour l’utiliser — un navigateur, `curl`, `git`, un IDE — font passer leur trafic par le tunnel ; le reste passe directement. Aucune carte réseau virtuelle n’est installée."
    - q: "Que faut-il sur le serveur ?"
      a: "Un serveur OpenSSH ordinaire et un compte autorisé à transférer des ports TCP. Rien n’est à installer sur le serveur ; tout VPS ou serveur domestique accessible en SSH convient."
    - q: "Puis-je me connecter avec un mot de passe ?"
      a: "Non, Shprot utilise uniquement des clés SSH. Elles sont plus sûres que les mots de passe et permettent au tunnel de se reconnecter sans intervention. La clé se génère dans l’application en un clic."
    - q: "Quelles applications peuvent utiliser le proxy ?"
      a: "Les navigateurs, `curl`, `git`, les gestionnaires de paquets et toute application dotée de paramètres de proxy SOCKS5 ou HTTP ou prenant en charge les variables `ALL_PROXY` / `HTTP_PROXY`. Utilisez `socks5h://` pour que les noms de sites soient résolus sur le serveur."
    - q: "Comment Shprot vérifie-t-il le tunnel ?"
      a: "De temps en temps, il ouvre via le proxy un site populaire choisi au hasard. Si cela échoue alors qu’Internet est disponible, le tunnel est redémarré."
    - q: "Est-ce gratuit ?"
      a: "Oui. Shprot est un logiciel libre et open source sous licence GNU GPL v3, sans compte, abonnement ni publicité."
    - q: "Existe-t-il une version macOS ?"
      a: "Pas encore. Shprot prend actuellement en charge Windows 10 / 11 et Linux."

footer:
  license: "Shprot est un logiciel libre sous licence GNU GPL v3."
  source: "Code source"
  issues: "Signaler un problème"
---
