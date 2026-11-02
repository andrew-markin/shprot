---
title: "Shprot — proxy SOCKS5 sobre SSH que se reconecta solo"
description: "Aplicación de escritorio gratuita para Windows y Linux que mantiene activo un proxy SOCKS5 y HTTP sobre SSH: se reconecta tras cambios de red y suspensión, detecta túneles bloqueados, sin línea de comandos."
readme: "README.md"

ui:
  skip: "Ir al contenido"
  nav_features: "Funciones"
  nav_compare: "Por qué Shprot"
  nav_download: "Descargar"
  nav_faq: "Preguntas"
  language: "Idioma"
  github: "Código fuente en GitHub"

hero:
  title: "Un proxy SOCKS5 sobre SSH que no se desconecta"
  lead: "Shprot convierte cualquier servidor al que puedas acceder por SSH en un proxy SOCKS5 y HTTP estable. Vigila tu red, recupera el túnel tras cambiar de Wi-Fi o salir de la suspensión y te dice exactamente qué ha fallado, sin línea de comandos."
  download: "Descargar"
  download_hint: "Versión {version} para Windows y Linux"
  facts:
    - "Gratuito y de código abierto"
    - "Windows 10 / 11 y Linux"
    - "49 idiomas"
  screenshot_alt: "Ventana de configuración de Shprot"

features:
  title: "Todo lo que un túnel necesita para seguir funcionando"
  items:
    - id: reconnect
      title: "Resiste los cambios de red"
      text: "Cambias de Wi-Fi, conectas un cable o despiertas el portátil: Shprot lo detecta y recupera el túnel por sí solo."
    - id: health
      title: "Detecta fallos silenciosos"
      text: "Comprobaciones periódicas a través del propio proxy detectan un túnel que parece conectado pero no deja pasar tráfico, y lo reinician."
    - id: gentle
      title: "Cuida tu servidor"
      text: "Los reintentos se espacian poco a poco y, tras inicios de sesión fallidos, se repiten como mucho una vez por minuto, así que fail2ban no bloqueará tu IP por una clave incorrecta."
    - id: keys
      title: "Claves SSH en dos clics"
      text: "Genera o importa una clave y copia la parte pública para `authorized_keys` directamente desde la ventana de configuración."
    - id: trust
      title: "Verificación clara del servidor"
      text: "En la primera conexión, la huella de la clave del servidor aparece en un diálogo para que puedas comprobarla antes de confiar en él."
    - id: http
      title: "También proxy HTTP"
      text: "Las aplicaciones sin soporte para SOCKS5 pueden usar un proxy HTTP opcional que pasa por el mismo túnel."
    - id: tray
      title: "Vive en la bandeja"
      text: "El icono muestra si el túnel está activo, las notificaciones explican por qué no lo está y los registros están a un clic."
    - id: languages
      title: "Habla tu idioma"
      text: "La interfaz está traducida a 49 idiomas, con diseño de derecha a izquierda para árabe, persa, hebreo y urdu."

compare:
  title: "¿Por qué no simplemente `ssh -D`?"
  lead: "`ssh -D 1080 user@server` te da un proxy SOCKS5 con un solo comando, hasta que la conexión deja de funcionar sin avisar. Shprot ejecuta el mismo cliente OpenSSH y se encarga de todo lo que el comando por sí solo no hace."
  rows:
    - case: "Cambio de red o de Wi-Fi, salida de la suspensión"
      ssh: "Reinícialo tú mismo"
      shprot: "Se reconecta automáticamente"
    - case: "Túnel conectado pero sin tráfico"
      ssh: "Pasa desapercibido"
      shprot: "Lo detectan las comprobaciones a través del proxy"
    - case: "Errores de autenticación repetidos"
      ssh: "Depende de tus scripts"
      shprot: "Reintentos espaciados para que el servidor no bloquee tu IP"
    - case: "Claves del cliente"
      ssh: "`ssh-keygen` y archivos en `~/.ssh`"
      shprot: "Generar o importar, copiar la clave pública con un clic"
    - case: "Verificación de la identidad del servidor"
      ssh: "Pregunta en el terminal"
      shprot: "Diálogo con la huella de la clave del servidor"
    - case: "Aplicaciones sin soporte para SOCKS5"
      ssh: "Sin solución"
      shprot: "Proxy HTTP opcional por el mismo túnel"
    - case: "Estado del túnel"
      ssh: "Ventana de terminal"
      shprot: "Icono en la bandeja y notificaciones con el motivo del fallo"
  note: "Herramientas como `autossh` ayudan con los reinicios, pero siguen requiriendo configuración manual y no tienen interfaz."

uses:
  title: "Para qué sirve"
  items:
    - id: infra
      title: "Tu propia infraestructura"
      text: "Accede a paneles internos, entornos de pruebas y herramientas de administración que solo están disponibles desde la red de tu servidor."
    - id: wifi
      title: "Redes poco fiables"
      text: "Envía tu tráfico a través de tu propio servidor cuando uses Wi-Fi público en cafeterías, hoteles o aeropuertos."
    - id: laptop
      title: "Trabajo remoto con portátil"
      text: "Mantén un proxy estable en un equipo que se suspende y cambia de red todo el día."

how:
  title: "Listo en tres pasos"
  steps:
    - title: "Prepara un servidor"
      text: "Sirve cualquier servidor Linux con OpenSSH. No hay que instalar nada en él: un usuario dedicado que solo puede reenviar puertos se crea con unos pocos comandos."
    - title: "Conecta Shprot"
      text: "Introduce `user@server`, genera una clave y añade su parte pública a `authorized_keys` en el servidor. Comprueba una vez la huella del servidor."
    - title: "Dirige las aplicaciones al proxy"
      text: "Usa `socks5h://localhost:1080` en el navegador o en la configuración de proxy del sistema; el menú de la bandeja copia la dirección por ti."
  guide: "Guía paso a paso con protección del servidor"

tour:
  title: "Un vistazo más de cerca"
  items:
    - id: trust
      caption: "En la primera conexión ves la huella de la clave del servidor y decides si confiar en él."
    - id: tray
      caption: "El menú de la bandeja muestra el estado del túnel y copia direcciones de proxy listas para usar."

download:
  title: "Descargar Shprot"
  lead: "Versión {version}. Gratuito, de código abierto, sin cuentas ni suscripciones."
  windows:
    title: "Windows"
    text: "Windows 10 y 11, 64 bits. Para actualizar, instala la versión nueva encima de la anterior."
    button: "Instalador (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ y distribuciones compatibles."
    button: "Paquete (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ y distribuciones compatibles."
    button: "Paquete (.rpm)"
  requirements: "Shprot usa el cliente OpenSSH (`ssh` y `ssh-keygen`). Viene incluido en Windows 10 / 11 y en la mayoría de escritorios Linux; si falta, Shprot te dirá cómo añadirlo."
  verify: "Cada versión incluye un archivo `SHA256SUMS` para comprobar que la descarga no ha sido alterada."
  all_releases: "Todas las versiones y cambios"

faq:
  title: "Preguntas y respuestas"
  items:
    - q: "¿Shprot es una VPN?"
      a: "No, es un proxy. Solo las aplicaciones configuradas para usarlo (un navegador, `curl`, `git`, un IDE) envían tráfico por el túnel; todo lo demás va directo. No se instalan adaptadores de red virtuales."
    - q: "¿Qué necesito en el servidor?"
      a: "Un servidor OpenSSH normal y una cuenta con permiso para reenviar puertos TCP. No hay que instalar nada en el servidor; sirve cualquier VPS o servidor doméstico accesible por SSH."
    - q: "¿Puedo iniciar sesión con contraseña?"
      a: "No, Shprot usa solo claves SSH. Las claves son más seguras que las contraseñas y permiten que el túnel se reconecte sin intervención. La clave se genera en la aplicación con un clic."
    - q: "¿Qué aplicaciones pueden usar el proxy?"
      a: "Navegadores, `curl`, `git`, gestores de paquetes y cualquier aplicación con ajustes de proxy SOCKS5 o HTTP o compatible con las variables `ALL_PROXY` / `HTTP_PROXY`. Usa `socks5h://` para que los nombres de los sitios se resuelvan en el servidor."
    - q: "¿Cómo comprueba Shprot el túnel?"
      a: "De vez en cuando abre a través del proxy un sitio web popular elegido al azar. Si falla mientras internet sí está disponible, el túnel se reinicia."
    - q: "¿Es gratuito?"
      a: "Sí. Shprot es software libre y de código abierto bajo la licencia GNU GPL v3, sin cuentas, suscripciones ni anuncios."
    - q: "¿Hay versión para macOS?"
      a: "Todavía no. Por ahora Shprot es compatible con Windows 10 / 11 y Linux."

footer:
  license: "Shprot es software libre con licencia GNU GPL v3."
  source: "Código fuente"
  issues: "Informar de un problema"
---
