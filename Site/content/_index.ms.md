---
title: "Shprot — proksi SOCKS5 melalui SSH yang menyambung semula sendiri"
description: "Aplikasi desktop percuma untuk Windows dan Linux yang memastikan proksi SOCKS5 dan HTTP melalui SSH terus berjalan: menyambung semula selepas pertukaran rangkaian dan tidur, mengesan terowong yang tersangkut, tanpa baris arahan."
readme: "README.md"

ui:
  skip: "Langkau ke kandungan"
  nav_features: "Ciri-ciri"
  nav_compare: "Mengapa Shprot"
  nav_download: "Muat turun"
  nav_faq: "Soalan lazim"
  language: "Bahasa"
  github: "Kod sumber di GitHub"

hero:
  title: "Proksi SOCKS5 melalui SSH yang kekal bersambung"
  lead: "Shprot menukar mana-mana pelayan yang boleh anda capai melalui SSH menjadi proksi SOCKS5 dan HTTP yang stabil. Ia memantau rangkaian, memulihkan terowong selepas menukar Wi-Fi atau bangun daripada mod tidur, dan memberitahu dengan tepat apa yang tidak kena — tanpa baris arahan."
  download: "Muat turun"
  download_hint: "Versi {version} untuk Windows dan Linux"
  facts:
    - "Percuma dan sumber terbuka"
    - "Windows 10 / 11 dan Linux"
    - "49 bahasa"
  screenshot_alt: "Tetingkap tetapan Shprot"

features:
  title: "Semua yang diperlukan terowong untuk terus berfungsi"
  items:
    - id: reconnect
      title: "Tahan pertukaran rangkaian"
      text: "Tukar Wi-Fi, sambung kabel atau bangunkan komputer riba — Shprot menyedarinya dan memulihkan terowong sendiri."
    - id: health
      title: "Mengesan kegagalan senyap"
      text: "Semakan berkala melalui proksi itu sendiri menemui terowong yang kelihatan bersambung tetapi tidak membenarkan trafik lalu, lalu memulakannya semula."
    - id: gentle
      title: "Berlembut dengan pelayan anda"
      text: "Cubaan semula makin jarang secara beransur-ansur, dan selepas log masuk gagal ia diulang paling kerap sekali seminit, jadi fail2ban tidak akan menyekat IP anda kerana kunci yang salah."
    - id: keys
      title: "Kunci SSH dalam dua klik"
      text: "Jana atau import kunci, kemudian salin bahagian awamnya untuk `authorized_keys` terus dari tetingkap tetapan."
    - id: trust
      title: "Pengesahan pelayan yang jelas"
      text: "Pada sambungan pertama, cap jari kunci hos pelayan dipaparkan dalam dialog supaya anda boleh menyemaknya sebelum mempercayai pelayan."
    - id: http
      title: "Proksi HTTP juga"
      text: "Aplikasi yang tidak menyokong SOCKS5 boleh menggunakan proksi HTTP pilihan yang melalui terowong yang sama."
    - id: tray
      title: "Tinggal di dulang sistem"
      text: "Ikon menunjukkan sama ada terowong terbuka, pemberitahuan menerangkan sebabnya jika tidak, dan log hanya sejauh satu klik."
    - id: languages
      title: "Bertutur dalam bahasa anda"
      text: "Antara muka diterjemahkan ke 49 bahasa, dengan susun atur kanan ke kiri untuk bahasa Arab, Parsi, Ibrani dan Urdu."

compare:
  title: "Mengapa tidak `ssh -D` sahaja?"
  lead: "`ssh -D 1080 user@server` memberi anda proksi SOCKS5 dengan satu arahan — sehingga sambungan berhenti berfungsi secara senyap. Shprot menjalankan klien OpenSSH yang sama dan menguruskan segala yang tidak dilakukan oleh arahan itu sendiri."
  rows:
    - case: "Tukar rangkaian atau Wi-Fi, bangun daripada mod tidur"
      ssh: "Mulakan semula sendiri"
      shprot: "Menyambung semula secara automatik"
    - case: "Terowong bersambung tetapi tiada trafik"
      ssh: "Tidak disedari"
      shprot: "Dikesan oleh semakan melalui proksi"
    - case: "Kegagalan pengesahan berulang"
      ssh: "Bergantung pada skrip anda"
      shprot: "Cubaan jarang supaya pelayan tidak menyekat IP anda"
    - case: "Kunci klien"
      ssh: "`ssh-keygen` dan fail dalam `~/.ssh`"
      shprot: "Jana atau import, salin kunci awam dengan satu klik"
    - case: "Semakan identiti pelayan"
      ssh: "Soalan dalam terminal"
      shprot: "Dialog dengan cap jari kunci hos"
    - case: "Aplikasi tanpa sokongan SOCKS5"
      ssh: "Tidak disokong"
      shprot: "Proksi HTTP pilihan melalui terowong yang sama"
    - case: "Status terowong"
      ssh: "Tetingkap terminal"
      shprot: "Ikon dulang dan pemberitahuan dengan punca kegagalan"
  note: "Alat seperti `autossh` membantu untuk memulakan semula, tetapi masih memerlukan persediaan manual dan tiada antara muka."

uses:
  title: "Sesuai untuk apa"
  items:
    - id: infra
      title: "Infrastruktur anda sendiri"
      text: "Capai papan pemuka dalaman, persekitaran ujian dan alat pentadbiran yang hanya boleh dibuka dari rangkaian pelayan anda."
    - id: wifi
      title: "Rangkaian yang tidak dipercayai"
      text: "Hantar trafik melalui pelayan anda sendiri apabila menggunakan Wi-Fi awam di kafe, hotel atau lapangan terbang."
    - id: laptop
      title: "Kerja jarak jauh dengan komputer riba"
      text: "Proksi yang stabil pada komputer yang tidur dan menukar rangkaian sepanjang hari."

how:
  title: "Sedia dalam tiga langkah"
  steps:
    - title: "Sediakan pelayan"
      text: "Mana-mana pelayan Linux dengan OpenSSH boleh digunakan. Tiada apa-apa dipasang padanya — pengguna khas yang hanya boleh memajukan port dicipta dengan beberapa arahan."
    - title: "Sambungkan Shprot"
      text: "Masukkan `user@server`, jana kunci dan tambah bahagian awamnya ke `authorized_keys` pada pelayan. Semak cap jari pelayan sekali sahaja."
    - title: "Halakan aplikasi ke proksi"
      text: "Gunakan `socks5h://localhost:1080` dalam pelayar atau tetapan proksi sistem — menu dulang menyalin alamat untuk anda."
  guide: "Panduan langkah demi langkah dengan pengukuhan pelayan"

tour:
  title: "Lihat lebih dekat"
  items:
    - id: trust
      caption: "Pada sambungan pertama anda melihat cap jari kunci pelayan dan memutuskan sama ada mempercayainya."
    - id: tray
      caption: "Menu dulang menunjukkan status terowong dan menyalin alamat proksi yang sedia digunakan."

download:
  title: "Muat turun Shprot"
  lead: "Versi {version}. Percuma, sumber terbuka, tanpa akaun atau langganan."
  windows:
    title: "Windows"
    text: "Windows 10 dan 11, 64-bit. Untuk mengemas kini, pasang versi baharu di atas versi lama."
    button: "Pemasang (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ dan pengedaran yang serasi."
    button: "Pakej (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ dan pengedaran yang serasi."
    button: "Pakej (.rpm)"
  requirements: "Shprot menggunakan klien OpenSSH (`ssh` dan `ssh-keygen`). Ia disertakan dalam Windows 10 / 11 dan kebanyakan desktop Linux; jika tiada, Shprot akan memberitahu cara menambahnya."
  verify: "Setiap keluaran disertakan fail `SHA256SUMS` untuk menyemak bahawa fail yang dimuat turun tidak diubah."
  all_releases: "Semua keluaran dan perubahan"

faq:
  title: "Soalan dan jawapan"
  items:
    - q: "Adakah Shprot VPN?"
      a: "Tidak, ia ialah proksi. Hanya aplikasi yang ditetapkan untuk menggunakannya — pelayar, `curl`, `git`, IDE — menghantar trafik melalui terowong; yang lain terus. Tiada penyesuai rangkaian maya dipasang."
    - q: "Apakah yang saya perlukan pada pelayan?"
      a: "Pelayan OpenSSH biasa dan akaun yang dibenarkan memajukan port TCP. Tiada apa-apa perlu dipasang pada pelayan; mana-mana VPS atau pelayan rumah yang boleh dicapai melalui SSH sesuai digunakan."
    - q: "Bolehkah saya log masuk dengan kata laluan?"
      a: "Tidak, Shprot hanya menggunakan kunci SSH. Kunci lebih selamat daripada kata laluan dan membolehkan terowong menyambung semula tanpa campur tangan anda. Kunci dijana dalam aplikasi dengan satu klik."
    - q: "Aplikasi apakah yang boleh menggunakan proksi?"
      a: "Pelayar, `curl`, `git`, pengurus pakej dan mana-mana aplikasi yang mempunyai tetapan proksi SOCKS5 atau HTTP atau menyokong pemboleh ubah `ALL_PROXY` / `HTTP_PROXY`. Gunakan `socks5h://` supaya nama laman diselesaikan pada pelayan."
    - q: "Bagaimanakah Shprot menyemak terowong?"
      a: "Sekali-sekala ia membuka laman web popular yang dipilih secara rawak melalui proksi. Jika gagal sedangkan internet tersedia, terowong dimulakan semula."
    - q: "Adakah ia percuma?"
      a: "Ya. Shprot ialah perisian bebas dan sumber terbuka di bawah lesen GNU GPL v3, tanpa akaun, langganan atau iklan."
    - q: "Adakah versi macOS?"
      a: "Belum lagi. Buat masa ini Shprot menyokong Windows 10 / 11 dan Linux."

footer:
  license: "Shprot ialah perisian bebas di bawah lesen GNU GPL v3."
  source: "Kod sumber"
  issues: "Laporkan masalah"
---
