---
title: "Shprot — kendi kendine yeniden bağlanan SSH üzerinden SOCKS5 proxy"
description: "Windows ve Linux için SSH üzerinden SOCKS5 ve HTTP proxy'yi çalışır tutan ücretsiz masaüstü uygulaması: ağ değişikliği ve uyku sonrası yeniden bağlanır, takılan tünelleri fark eder, komut satırı gerekmez."
readme: "README.md"

ui:
  skip: "İçeriğe geç"
  nav_features: "Özellikler"
  nav_compare: "Neden Shprot"
  nav_download: "İndir"
  nav_faq: "SSS"
  language: "Dil"
  github: "GitHub'da kaynak kodu"

hero:
  title: "Bağlantıyı koruyan, SSH üzerinden SOCKS5 proxy"
  lead: "Shprot, SSH ile erişebildiğiniz her sunucuyu kararlı bir SOCKS5 ve HTTP proxy'ye dönüştürür. Ağınızı izler, Wi-Fi değişikliğinden ve uykudan sonra tüneli geri getirir ve neyin ters gittiğini tam olarak söyler — komut satırı olmadan."
  download: "İndir"
  download_hint: "Windows ve Linux için {version} sürümü"
  facts:
    - "Ücretsiz ve açık kaynak"
    - "Windows 10 / 11 ve Linux"
    - "49 dil"
  screenshot_alt: "Shprot ayarlar penceresi"

features:
  title: "Bir tünelin çalışmaya devam etmesi için gereken her şey"
  items:
    - id: reconnect
      title: "Ağ değişikliklerine dayanır"
      text: "Wi-Fi'yi değiştirin, kablo takın ya da dizüstü bilgisayarı uykudan uyandırın — Shprot bunu fark eder ve tüneli kendiliğinden geri getirir."
    - id: health
      title: "Sessiz arızaları yakalar"
      text: "Proxy'nin kendisi üzerinden yapılan düzenli kontroller, bağlı görünen ama trafik geçirmeyen bir tüneli bulur ve yeniden başlatır."
    - id: gentle
      title: "Sunucunuza nazik"
      text: "Yeniden denemeler giderek seyrekleşir ve başarısız oturum açmalardan sonra en fazla dakikada bir yapılır; böylece fail2ban yanlış bir anahtar yüzünden IP adresinizi engellemez."
    - id: keys
      title: "İki tıkla SSH anahtarları"
      text: "Bir anahtar oluşturun ya da içe aktarın ve ortak kısmını `authorized_keys` için doğrudan ayarlar penceresinden kopyalayın."
    - id: trust
      title: "Açık sunucu doğrulaması"
      text: "İlk bağlantıda sunucunun ana bilgisayar anahtarının parmak izi bir iletişim kutusunda gösterilir; sunucuya güvenmeden önce kontrol edebilirsiniz."
    - id: http
      title: "HTTP proxy de var"
      text: "SOCKS5 desteklemeyen uygulamalar, aynı tünelden geçen isteğe bağlı bir HTTP proxy kullanabilir."
    - id: tray
      title: "Sistem tepsisinde yaşar"
      text: "Simge tünelin açık olup olmadığını gösterir, bildirimler neden açık olmadığını açıklar ve günlükler bir tık uzağınızdadır."
    - id: languages
      title: "Sizin dilinizi konuşur"
      text: "Arayüz 49 dile çevrilmiştir; Arapça, Farsça, İbranice ve Urduca için sağdan sola düzen kullanılır."

compare:
  title: "Neden sadece `ssh -D` değil?"
  lead: "`ssh -D 1080 user@server` tek komutla bir SOCKS5 proxy sağlar — ta ki bağlantı sessizce çalışmayı bırakana kadar. Shprot aynı OpenSSH istemcisini çalıştırır ve komutun tek başına yapmadığı her şeyle ilgilenir."
  rows:
    - case: "Ağ değişikliği, Wi-Fi değişimi, uykudan uyanma"
      ssh: "Kendiniz yeniden başlatın"
      shprot: "Otomatik olarak yeniden bağlanır"
    - case: "Tünel bağlı ama trafik geçmiyor"
      ssh: "Fark edilmez"
      shprot: "Proxy üzerinden yapılan kontrollerle tespit edilir"
    - case: "Tekrarlanan kimlik doğrulama hataları"
      ssh: "Betiklerinize kalmış"
      shprot: "Seyrek denemeler, böylece sunucu IP'nizi engellemez"
    - case: "İstemci anahtarları"
      ssh: "`ssh-keygen` ve `~/.ssh` içindeki dosyalar"
      shprot: "Oluşturun ya da içe aktarın, ortak anahtarı tek tıkla kopyalayın"
    - case: "Sunucu kimliğinin doğrulanması"
      ssh: "Terminalde soru"
      shprot: "Anahtar parmak izini gösteren iletişim kutusu"
    - case: "SOCKS5 desteklemeyen uygulamalar"
      ssh: "Karşılanmaz"
      shprot: "Aynı tünelden isteğe bağlı HTTP proxy"
    - case: "Tünel durumu"
      ssh: "Terminal penceresi"
      shprot: "Tepsi simgesi ve hata nedenini içeren bildirimler"
  note: "`autossh` gibi araçlar yeniden başlatmaya yardımcı olur, ancak yine de elle yapılandırma gerektirir ve arayüzleri yoktur."

uses:
  title: "Ne işe yarar"
  items:
    - id: infra
      title: "Kendi altyapınız"
      text: "Yalnızca sunucunuzun ağından erişilebilen dahili panellere, test ortamlarına ve yönetim araçlarına ulaşın."
    - id: wifi
      title: "Güvenilmeyen ağlar"
      text: "Kafelerde, otellerde ya da havalimanlarında halka açık Wi-Fi kullanırken trafiğinizi kendi sunucunuzdan geçirin."
    - id: laptop
      title: "Dizüstü bilgisayarla uzaktan çalışma"
      text: "Gün boyu uykuya geçen ve ağ değiştiren bir bilgisayarda kararlı bir proxy."

how:
  title: "Üç adımda hazır"
  steps:
    - title: "Bir sunucu hazırlayın"
      text: "OpenSSH yüklü herhangi bir Linux sunucusu yeterlidir. Üzerine hiçbir şey kurulmaz — yalnızca port yönlendirebilen ayrı bir kullanıcı birkaç komutla oluşturulur."
    - title: "Shprot'u bağlayın"
      text: "`user@server` girin, bir anahtar oluşturun ve ortak kısmını sunucudaki `authorized_keys` dosyasına ekleyin. Sunucunun parmak izini bir kez kontrol edin."
    - title: "Uygulamaları proxy'ye yönlendirin"
      text: "Tarayıcıda ya da sistem proxy ayarlarında `socks5h://localhost:1080` kullanın — tepsi menüsü adresi sizin için kopyalar."
  guide: "Sunucu güçlendirmesiyle adım adım kurulum kılavuzu"

tour:
  title: "Daha yakından"
  items:
    - id: trust
      caption: "İlk bağlantıda sunucu anahtarının parmak izini görür ve ona güvenip güvenmeyeceğinize karar verirsiniz."
    - id: tray
      caption: "Tepsi menüsü tünel durumunu gösterir ve kullanıma hazır proxy adreslerini kopyalar."

download:
  title: "Shprot'u indirin"
  lead: "{version} sürümü. Ücretsiz, açık kaynak, hesap ya da abonelik yok."
  windows:
    title: "Windows"
    text: "Windows 10 ve 11, 64 bit. Güncellemek için yeni sürümü eskisinin üzerine kurun."
    button: "Yükleyici (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ ve uyumlu dağıtımlar."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ ve uyumlu dağıtımlar."
    button: "Paket (.rpm)"
  requirements: "Shprot, OpenSSH istemcisini (`ssh` ve `ssh-keygen`) kullanır. Windows 10 / 11 ve çoğu Linux masaüstünde hazır gelir; eksikse Shprot nasıl ekleyeceğinizi söyler."
  verify: "Her sürüm, indirilen dosyanın değiştirilmediğini kontrol etmek için bir `SHA256SUMS` dosyası içerir."
  all_releases: "Tüm sürümler ve değişiklikler"

faq:
  title: "Sorular ve yanıtlar"
  items:
    - q: "Shprot bir VPN mi?"
      a: "Hayır, bir proxy. Yalnızca onu kullanacak şekilde ayarlanmış uygulamalar — bir tarayıcı, `curl`, `git`, bir IDE — trafiği tünelden gönderir; geri kalan her şey doğrudan gider. Sanal ağ bağdaştırıcısı kurulmaz."
    - q: "Sunucuda neye ihtiyacım var?"
      a: "Sıradan bir OpenSSH sunucusu ve TCP port yönlendirmesine izin verilen bir hesap. Sunucuya hiçbir şey kurmanız gerekmez; SSH ile erişebildiğiniz herhangi bir VPS ya da ev sunucusu olur."
    - q: "Parolayla giriş yapabilir miyim?"
      a: "Hayır, Shprot yalnızca SSH anahtarları kullanır. Anahtarlar parolalardan daha güvenlidir ve tünelin siz müdahale etmeden yeniden bağlanmasını sağlar. Anahtar uygulamada tek tıkla oluşturulur."
    - q: "Hangi uygulamalar proxy'yi kullanabilir?"
      a: "Tarayıcılar, `curl`, `git`, paket yöneticileri ve SOCKS5 ya da HTTP proxy ayarları olan veya `ALL_PROXY` / `HTTP_PROXY` değişkenlerini destekleyen her uygulama. Site adlarının sunucuda çözümlenmesi için `socks5h://` kullanın."
    - q: "Shprot tüneli nasıl kontrol eder?"
      a: "Zaman zaman proxy üzerinden rastgele seçilen popüler bir web sitesini açar. İnternet erişilebilirken bu başarısız olursa tünel yeniden başlatılır."
    - q: "Ücretsiz mi?"
      a: "Evet. Shprot, GNU GPL v3 lisansıyla sunulan özgür ve açık kaynaklı bir yazılımdır; hesap, abonelik ya da reklam yoktur."
    - q: "macOS sürümü var mı?"
      a: "Henüz yok. Shprot şu anda Windows 10 / 11 ve Linux'u destekliyor."

footer:
  license: "Shprot, GNU GPL v3 lisanslı özgür bir yazılımdır."
  source: "Kaynak kodu"
  issues: "Sorun bildir"
---
