---
title: "Shprot — o‘zi qayta ulanadigan SSH orqali SOCKS5 proksi"
description: "Windows va Linux uchun bepul ilova, SSH orqali SOCKS5 va HTTP proksini ishlab turgan holda saqlaydi: tarmoq almashgani va uyqudan keyin qayta ulanadi, qotib qolgan tunnelni sezadi, buyruq satrisiz."
readme: "README.md"

ui:
  skip: "Asosiy qismga o‘tish"
  nav_features: "Imkoniyatlar"
  nav_compare: "Nega Shprot"
  nav_download: "Yuklab olish"
  nav_faq: "Savollar"
  language: "Til"
  github: "GitHub’dagi manba kodi"

hero:
  title: "Uzilib qolmaydigan SSH orqali SOCKS5 proksi"
  lead: "Shprot SSH orqali kira oladigan istalgan serverni barqaror SOCKS5 va HTTP proksiga aylantiradi. U tarmoqni kuzatadi, Wi-Fi almashgani va uyqudan uyg‘ongandan keyin tunnelni tiklaydi hamda nima noto‘g‘ri bo‘lganini aniq aytadi — buyruq satrisiz."
  download: "Yuklab olish"
  download_hint: "Windows va Linux uchun {version} versiya"
  facts:
    - "Bepul va ochiq kodli"
    - "Windows 10 / 11 va Linux"
    - "49 til"
  screenshot_alt: "Shprot sozlamalar oynasi"

features:
  title: "Tunnel ishlashda davom etishi uchun kerak bo‘lgan hamma narsa"
  items:
    - id: reconnect
      title: "Tarmoq almashishiga chidaydi"
      text: "Wi-Fi’ni almashtirdingiz, kabel uladingiz yoki noutbukni uyqudan uyg‘otdingiz — Shprot buni sezadi va tunnelni o‘zi tiklaydi."
    - id: health
      title: "Jimjit nosozliklarni ushlaydi"
      text: "Proksining o‘zi orqali muntazam tekshiruvlar ulangan ko‘rinadigan, lekin trafik o‘tkazmaydigan tunnelni topib, uni qayta ishga tushiradi."
    - id: gentle
      title: "Serveringizni ayaydi"
      text: "Qayta urinishlar asta-sekin siyraklashadi, kirish muvaffaqiyatsiz bo‘lgach esa daqiqasiga bir martadan ko‘p takrorlanmaydi, shuning uchun noto‘g‘ri kalit sababli fail2ban IP manzilingizni bloklamaydi."
    - id: keys
      title: "Ikki bosishda SSH kalitlari"
      text: "Kalit yarating yoki import qiling va uning ochiq qismini `authorized_keys` uchun to‘g‘ridan-to‘g‘ri sozlamalar oynasidan nusxalang."
    - id: trust
      title: "Serverni tushunarli tekshirish"
      text: "Birinchi ulanishda server kalitining barmoq izi dialogda ko‘rsatiladi, serverga ishonishdan oldin uni tekshirib olishingiz mumkin."
    - id: http
      title: "HTTP proksi ham bor"
      text: "SOCKS5’ni qo‘llamaydigan dasturlar xuddi shu tunneldan o‘tadigan qo‘shimcha HTTP proksidan foydalanishi mumkin."
    - id: tray
      title: "Treyda yashaydi"
      text: "Belgi tunnel ochiq yoki yo‘qligini ko‘rsatadi, bildirishnomalar nega ochilmaganini tushuntiradi, jurnal esa bir bosishda ochiladi."
    - id: languages
      title: "Sizning tilingizda gapiradi"
      text: "Interfeys 49 tilga tarjima qilingan, arab, fors, ivrit va urdu tillari uchun o‘ngdan chapga joylashuv bilan."

compare:
  title: "Nega shunchaki `ssh -D` emas?"
  lead: "`ssh -D 1080 user@server` bitta buyruq bilan SOCKS5 proksi beradi — ulanish sezdirmay ishlashdan to‘xtaguncha. Shprot xuddi shu OpenSSH mijozini ishga tushiradi va bitta buyruq o‘zi qilmaydigan hamma narsani o‘z zimmasiga oladi."
  rows:
    - case: "Tarmoq yoki Wi-Fi almashishi, uyqudan uyg‘onish"
      ssh: "O‘zingiz qayta ishga tushirasiz"
      shprot: "Avtomatik qayta ulanadi"
    - case: "Tunnel ulangan, lekin trafik o‘tmaydi"
      ssh: "Sezilmay qoladi"
      shprot: "Proksi orqali tekshiruvlar bilan aniqlanadi"
    - case: "Takroriy autentifikatsiya xatolari"
      ssh: "Skriptlaringizga bog‘liq"
      shprot: "Siyrak urinishlar, server IP manzilingizni bloklamasligi uchun"
    - case: "Mijoz kalitlari"
      ssh: "`ssh-keygen` va `~/.ssh` dagi fayllar"
      shprot: "Yaratish yoki import qilish, ochiq kalitni bir bosishda nusxalash"
    - case: "Server haqiqiyligini tekshirish"
      ssh: "Terminaldagi savol"
      shprot: "Kalit barmoq izi ko‘rsatilgan dialog"
    - case: "SOCKS5’ni qo‘llamaydigan dasturlar"
      ssh: "Qo‘llab-quvvatlanmaydi"
      shprot: "Xuddi shu tunnel orqali qo‘shimcha HTTP proksi"
    - case: "Tunnel holati"
      ssh: "Terminal oynasi"
      shprot: "Trey belgisi va nosozlik sababi ko‘rsatilgan bildirishnomalar"
  note: "`autossh` kabi vositalar qayta ishga tushirishda yordam beradi, lekin baribir qo‘lda sozlashni talab qiladi va interfeysi yo‘q."

uses:
  title: "Nima uchun foydali"
  items:
    - id: infra
      title: "O‘z infratuzilmangiz"
      text: "Faqat serveringiz tarmog‘idan ochiladigan ichki panellar, sinov muhitlari va boshqaruv vositalariga kiring."
    - id: wifi
      title: "Ishonchsiz tarmoqlar"
      text: "Kafe, mehmonxona yoki aeroportdagi ommaviy Wi-Fi’dan foydalanganda trafikni o‘z serveringiz orqali yuboring."
    - id: laptop
      title: "Noutbukda masofaviy ish"
      text: "Kun bo‘yi uxlab, tarmoq almashtiradigan kompyuterda barqaror proksi."

how:
  title: "Uch qadamda tayyor"
  steps:
    - title: "Serverni tayyorlang"
      text: "OpenSSH o‘rnatilgan istalgan Linux server mos keladi. Unga hech narsa o‘rnatilmaydi — faqat portlarni yo‘naltira oladigan alohida foydalanuvchi bir nechta buyruq bilan yaratiladi."
    - title: "Shprot’ni ulang"
      text: "`user@server` kiriting, kalit yarating va uning ochiq qismini serverdagi `authorized_keys` ga qo‘shing. Server barmoq izini bir marta tekshiring."
    - title: "Dasturlarni proksiga yo‘naltiring"
      text: "Brauzerda yoki tizimning proksi sozlamalarida `socks5h://localhost:1080` dan foydalaning — trey menyusi manzilni siz uchun nusxalaydi."
  guide: "Serverni himoyalash bilan bosqichma-bosqich qo‘llanma"

tour:
  title: "Yaqinroqdan"
  items:
    - id: trust
      caption: "Birinchi ulanishda server kalitining barmoq izini ko‘rasiz va unga ishonish-ishonmaslikni hal qilasiz."
    - id: tray
      caption: "Trey menyusi tunnel holatini ko‘rsatadi va tayyor proksi manzillarini nusxalaydi."

download:
  title: "Shprot’ni yuklab oling"
  lead: "{version} versiya. Bepul, ochiq kodli, hisob va obunalarsiz."
  windows:
    title: "Windows"
    text: "Windows 10 va 11, 64 bit. Yangilash uchun yangi versiyani eskisining ustiga o‘rnating."
    button: "O‘rnatuvchi (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ va mos distributivlar."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ va mos distributivlar."
    button: "Paket (.rpm)"
  requirements: "Shprot OpenSSH mijozidan (`ssh` va `ssh-keygen`) foydalanadi. U Windows 10 / 11 va ko‘pchilik Linux ish stollarida mavjud; agar bo‘lmasa, Shprot uni qanday qo‘shishni aytadi."
  verify: "Har bir relizda yuklab olingan fayl o‘zgartirilmaganini tekshirish uchun `SHA256SUMS` fayli bor."
  all_releases: "Barcha relizlar va o‘zgarishlar"

faq:
  title: "Savollar va javoblar"
  items:
    - q: "Shprot VPN’mi?"
      a: "Yo‘q, bu proksi. Tunnel orqali faqat undan foydalanishga sozlangan dasturlarning — brauzer, `curl`, `git`, IDE — trafigi o‘tadi; qolgani to‘g‘ridan-to‘g‘ri ketadi. Virtual tarmoq adapterlari o‘rnatilmaydi."
    - q: "Serverda nima kerak?"
      a: "Oddiy OpenSSH server va TCP portlarini yo‘naltirishga ruxsati bor hisob. Serverga hech narsa o‘rnatish shart emas; SSH orqali kirish mumkin bo‘lgan istalgan VPS yoki uy serveri mos keladi."
    - q: "Parol bilan kirsam bo‘ladimi?"
      a: "Yo‘q, Shprot faqat SSH kalitlaridan foydalanadi. Kalitlar paroldan xavfsizroq va tunnelga sizning ishtirokingizsiz qayta ulanish imkonini beradi. Kalit ilovada bir bosishda yaratiladi."
    - q: "Qaysi dasturlar proksidan foydalana oladi?"
      a: "Brauzerlar, `curl`, `git`, paket menejerlari va SOCKS5 yoki HTTP proksi sozlamalari bor yoki `ALL_PROXY` / `HTTP_PROXY` o‘zgaruvchilarini qo‘llaydigan istalgan dastur. Sayt nomlari serverda aniqlanishi uchun `socks5h://` dan foydalaning."
    - q: "Shprot tunnelni qanday tekshiradi?"
      a: "Vaqti-vaqti bilan u proksi orqali tasodifiy tanlangan mashhur saytni ochadi. Internet mavjud bo‘lsa-yu, bu muvaffaqiyatsiz bo‘lsa, tunnel qayta ishga tushiriladi."
    - q: "Bu bepulmi?"
      a: "Ha. Shprot — GNU GPL v3 litsenziyasi ostidagi erkin va ochiq kodli dasturiy ta’minot, hisoblar, obunalar va reklamalarsiz."
    - q: "macOS versiyasi bormi?"
      a: "Hozircha yo‘q. Hozir Shprot Windows 10 / 11 va Linux’ni qo‘llab-quvvatlaydi."

footer:
  license: "Shprot — GNU GPL v3 litsenziyasi ostidagi erkin dasturiy ta’minot."
  source: "Manba kodi"
  issues: "Muammo haqida xabar berish"
---
