---
title: "Shprot — özü yenidən qoşulan SSH üzərindən SOCKS5 proksi"
description: "Windows və Linux üçün pulsuz masaüstü tətbiqi, SSH üzərindən SOCKS5 və HTTP proksini işlək saxlayır: şəbəkə dəyişikliyi və yuxu rejimindən sonra yenidən qoşulur, ilişib qalan tuneli aşkar edir, əmr sətri olmadan."
readme: "README.md"

ui:
  skip: "Məzmuna keç"
  nav_features: "İmkanlar"
  nav_compare: "Niyə Shprot"
  nav_download: "Endir"
  nav_faq: "Suallar"
  language: "Dil"
  github: "GitHub-da mənbə kodu"

hero:
  title: "Bağlantını itirməyən SSH üzərindən SOCKS5 proksi"
  lead: "Shprot SSH ilə daxil ola bildiyiniz istənilən serveri sabit SOCKS5 və HTTP proksiyə çevirir. O, şəbəkəni izləyir, Wi-Fi dəyişdikdən və yuxu rejimindən oyandıqdan sonra tuneli bərpa edir və nəyin səhv getdiyini dəqiq deyir — əmr sətri olmadan."
  download: "Endir"
  download_hint: "Windows və Linux üçün {version} versiyası"
  facts:
    - "Pulsuz və açıq mənbəli"
    - "Windows 10 / 11 və Linux"
    - "49 dil"
  screenshot_alt: "Shprot tənzimləmələr pəncərəsi"

features:
  title: "Tunelin işləməyə davam etməsi üçün lazım olan hər şey"
  items:
    - id: reconnect
      title: "Şəbəkə dəyişikliyinə davam gətirir"
      text: "Wi-Fi-ı dəyişdiniz, kabel taxdınız və ya noutbuku yuxudan oyatdınız — Shprot bunu görür və tuneli özü bərpa edir."
    - id: health
      title: "Səssiz nasazlıqları tutur"
      text: "Proksinin özü vasitəsilə müntəzəm yoxlamalar qoşulmuş görünən, lakin trafik ötürməyən tuneli tapır və onu yenidən başladır."
    - id: gentle
      title: "Serverinizi qoruyur"
      text: "Təkrar cəhdlər tədricən seyrəlir, uğursuz girişlərdən sonra isə dəqiqədə bir dəfədən tez-tez təkrarlanmır, buna görə fail2ban səhv açar üzündən IP ünvanınızı bloklamaz."
    - id: keys
      title: "İki kliklə SSH açarları"
      text: "Açar yaradın və ya idxal edin, onun açıq hissəsini `authorized_keys` üçün birbaşa tənzimləmələr pəncərəsindən kopyalayın."
    - id: trust
      title: "Serverin aydın yoxlanışı"
      text: "İlk qoşulmada serverin açarının barmaq izi dialoqda göstərilir ki, serverə etibar etməzdən əvvəl onu yoxlaya biləsiniz."
    - id: http
      title: "HTTP proksi də var"
      text: "SOCKS5 dəstəkləməyən proqramlar eyni tuneldən keçən əlavə HTTP proksidən istifadə edə bilər."
    - id: tray
      title: "Sistem sinisində yaşayır"
      text: "Nişan tunelin açıq olub-olmadığını göstərir, bildirişlər niyə açılmadığını izah edir, jurnal isə bir klik uzaqlıqdadır."
    - id: languages
      title: "Sizin dilinizdə danışır"
      text: "İnterfeys 49 dilə tərcümə edilib, ərəb, fars, ivrit və urdu dilləri üçün sağdan sola düzülüşlə."

compare:
  title: "Niyə sadəcə `ssh -D` yox?"
  lead: "`ssh -D 1080 user@server` bir əmrlə SOCKS5 proksi verir — bağlantı xəbərsiz işləməyi dayandırana qədər. Shprot eyni OpenSSH müştərisini işə salır və tək əmrin etmədiyi hər şeylə məşğul olur."
  rows:
    - case: "Şəbəkə və ya Wi-Fi dəyişikliyi, yuxudan oyanma"
      ssh: "Özünüz yenidən başladın"
      shprot: "Avtomatik yenidən qoşulur"
    - case: "Tunel qoşulub, amma trafik keçmir"
      ssh: "Diqqətdən kənarda qalır"
      shprot: "Proksi vasitəsilə yoxlamalarla aşkarlanır"
    - case: "Təkrarlanan autentifikasiya xətaları"
      ssh: "Skriptlərinizdən asılıdır"
      shprot: "Seyrək cəhdlər ki, server IP-nizi bloklamasın"
    - case: "Müştəri açarları"
      ssh: "`ssh-keygen` və `~/.ssh` içindəki fayllar"
      shprot: "Yaradın və ya idxal edin, açıq açarı bir kliklə kopyalayın"
    - case: "Serverin kimliyinin yoxlanması"
      ssh: "Terminalda sual"
      shprot: "Açarın barmaq izi olan dialoq"
    - case: "SOCKS5 dəstəkləməyən proqramlar"
      ssh: "Dəstəklənmir"
      shprot: "Eyni tuneldən əlavə HTTP proksi"
    - case: "Tunelin vəziyyəti"
      ssh: "Terminal pəncərəsi"
      shprot: "Sini nişanı və nasazlığın səbəbi göstərilən bildirişlər"
  note: "`autossh` kimi alətlər yenidən başlatmağa kömək edir, lakin yenə də əl ilə tənzimləmə tələb edir və interfeysi yoxdur."

uses:
  title: "Nə üçün yararlıdır"
  items:
    - id: infra
      title: "Öz infrastrukturunuz"
      text: "Yalnız serverinizin şəbəkəsindən açılan daxili panellərə, test mühitlərinə və idarəetmə alətlərinə daxil olun."
    - id: wifi
      title: "Etibarsız şəbəkələr"
      text: "Kafe, otel və ya hava limanında ictimai Wi-Fi-dan istifadə edərkən trafiki öz serveriniz vasitəsilə göndərin."
    - id: laptop
      title: "Noutbukla uzaqdan iş"
      text: "Bütün gün yuxuya gedən və şəbəkə dəyişən kompüterdə sabit proksi."

how:
  title: "Üç addımda hazır"
  steps:
    - title: "Serveri hazırlayın"
      text: "OpenSSH olan istənilən Linux server uyğundur. Ona heç nə quraşdırılmır — yalnız portları yönləndirə bilən ayrıca istifadəçi bir neçə əmrlə yaradılır."
    - title: "Shprot-u qoşun"
      text: "`user@server` daxil edin, açar yaradın və onun açıq hissəsini serverdə `authorized_keys` faylına əlavə edin. Serverin barmaq izini bir dəfə yoxlayın."
    - title: "Proqramları proksiyə yönəldin"
      text: "Brauzerdə və ya sistemin proksi tənzimləmələrində `socks5h://localhost:1080` istifadə edin — sini menyusu ünvanı sizin üçün kopyalayır."
  guide: "Serverin qorunması ilə addım-addım təlimat"

tour:
  title: "Yaxından"
  items:
    - id: trust
      caption: "İlk qoşulmada server açarının barmaq izini görür və ona etibar edib-etməyəcəyinizə qərar verirsiniz."
    - id: tray
      caption: "Sini menyusu tunelin vəziyyətini göstərir və hazır proksi ünvanlarını kopyalayır."

download:
  title: "Shprot-u endirin"
  lead: "{version} versiyası. Pulsuz, açıq mənbəli, hesab və abunəlik olmadan."
  windows:
    title: "Windows"
    text: "Windows 10 və 11, 64 bit. Yeniləmək üçün yeni versiyanı köhnənin üzərinə quraşdırın."
    button: "Quraşdırıcı (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ və uyğun distributivlər."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ və uyğun distributivlər."
    button: "Paket (.rpm)"
  requirements: "Shprot OpenSSH müştərisindən (`ssh` və `ssh-keygen`) istifadə edir. O, Windows 10 / 11 və əksər Linux iş masalarında var; yoxdursa, Shprot onu necə əlavə edəcəyinizi deyəcək."
  verify: "Hər buraxılışda endirilən faylın dəyişdirilmədiyini yoxlamaq üçün `SHA256SUMS` faylı var."
  all_releases: "Bütün buraxılışlar və dəyişikliklər"

faq:
  title: "Suallar və cavablar"
  items:
    - q: "Shprot VPN-dirmi?"
      a: "Xeyr, bu proksidir. Tuneldən yalnız ondan istifadə üçün tənzimlənmiş proqramların — brauzer, `curl`, `git`, IDE — trafiki keçir; qalan hər şey birbaşa gedir. Virtual şəbəkə adapterləri quraşdırılmır."
    - q: "Serverdə nə lazımdır?"
      a: "Adi OpenSSH serveri və TCP portlarını yönləndirməyə icazəsi olan hesab. Serverə heç nə quraşdırmaq lazım deyil; SSH ilə əlçatan istənilən VPS və ya ev serveri uyğundur."
    - q: "Parolla daxil ola bilərəmmi?"
      a: "Xeyr, Shprot yalnız SSH açarlarından istifadə edir. Açarlar paroldan daha təhlükəsizdir və tunelə sizin müdaxiləniz olmadan yenidən qoşulmağa imkan verir. Açar tətbiqdə bir kliklə yaradılır."
    - q: "Hansı proqramlar proksidən istifadə edə bilər?"
      a: "Brauzerlər, `curl`, `git`, paket menecerləri və SOCKS5 və ya HTTP proksi tənzimləmələri olan, yaxud `ALL_PROXY` / `HTTP_PROXY` dəyişənlərini dəstəkləyən istənilən proqram. Sayt adlarının serverdə həll olunması üçün `socks5h://` istifadə edin."
    - q: "Shprot tuneli necə yoxlayır?"
      a: "Vaxtaşırı proksi vasitəsilə təsadüfi seçilmiş məşhur saytı açır. İnternet əlçatan olduğu halda bu alınmasa, tunel yenidən başladılır."
    - q: "Pulsuzdurmu?"
      a: "Bəli. Shprot GNU GPL v3 lisenziyası ilə azad və açıq mənbəli proqram təminatıdır, hesab, abunəlik və reklam olmadan."
    - q: "macOS versiyası varmı?"
      a: "Hələ ki yox. Hazırda Shprot Windows 10 / 11 və Linux-u dəstəkləyir."

footer:
  license: "Shprot GNU GPL v3 lisenziyası ilə azad proqram təminatıdır."
  source: "Mənbə kodu"
  issues: "Problem barədə bildir"
---
