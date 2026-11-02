---
title: "Shprot — proxy SOCKS5 lewat SSH yang tersambung ulang sendiri"
description: "Aplikasi desktop gratis untuk Windows dan Linux yang menjaga proxy SOCKS5 dan HTTP lewat SSH tetap berjalan: tersambung ulang setelah ganti jaringan dan tidur, mendeteksi terowongan yang macet, tanpa baris perintah."
readme: "README.md"

ui:
  skip: "Lompat ke konten"
  nav_features: "Fitur"
  nav_compare: "Mengapa Shprot"
  nav_download: "Unduh"
  nav_faq: "Tanya jawab"
  language: "Bahasa"
  github: "Kode sumber di GitHub"

hero:
  title: "Proxy SOCKS5 lewat SSH yang tetap tersambung"
  lead: "Shprot mengubah server apa pun yang bisa Anda akses lewat SSH menjadi proxy SOCKS5 dan HTTP yang stabil. Aplikasi ini memantau jaringan, memulihkan terowongan setelah ganti Wi-Fi atau bangun dari mode tidur, dan memberi tahu persis apa yang salah — tanpa baris perintah."
  download: "Unduh"
  download_hint: "Versi {version} untuk Windows dan Linux"
  facts:
    - "Gratis dan sumber terbuka"
    - "Windows 10 / 11 dan Linux"
    - "49 bahasa"
  screenshot_alt: "Jendela pengaturan Shprot"

features:
  title: "Semua yang dibutuhkan terowongan agar tetap berjalan"
  items:
    - id: reconnect
      title: "Tahan pergantian jaringan"
      text: "Ganti Wi-Fi, colok kabel, atau bangunkan laptop — Shprot menyadarinya dan memulihkan terowongan sendiri."
    - id: health
      title: "Menangkap gangguan senyap"
      text: "Pemeriksaan rutin melalui proxy itu sendiri menemukan terowongan yang tampak tersambung tetapi tidak meneruskan lalu lintas, lalu memulainya ulang."
    - id: gentle
      title: "Ramah terhadap server Anda"
      text: "Percobaan ulang makin lama makin jarang, dan setelah login gagal hanya diulang paling sering sekali per menit, sehingga fail2ban tidak memblokir IP Anda karena kunci yang salah."
    - id: keys
      title: "Kunci SSH dalam dua klik"
      text: "Buat atau impor kunci, lalu salin bagian publiknya untuk `authorized_keys` langsung dari jendela pengaturan."
    - id: trust
      title: "Verifikasi server yang jelas"
      text: "Pada sambungan pertama, sidik jari kunci host server ditampilkan dalam dialog agar Anda bisa memeriksanya sebelum memercayai server."
    - id: http
      title: "Ada proxy HTTP juga"
      text: "Aplikasi yang tidak mendukung SOCKS5 dapat memakai proxy HTTP opsional yang melewati terowongan yang sama."
    - id: tray
      title: "Tinggal di baki sistem"
      text: "Ikon menunjukkan apakah terowongan aktif, notifikasi menjelaskan mengapa tidak, dan log hanya sejauh satu klik."
    - id: languages
      title: "Berbicara dalam bahasa Anda"
      text: "Antarmuka diterjemahkan ke 49 bahasa, dengan tata letak kanan-ke-kiri untuk bahasa Arab, Persia, Ibrani, dan Urdu."

compare:
  title: "Mengapa tidak cukup `ssh -D`?"
  lead: "`ssh -D 1080 user@server` memberi Anda proxy SOCKS5 dengan satu perintah — sampai sambungannya diam-diam berhenti bekerja. Shprot menjalankan klien OpenSSH yang sama dan mengurus semua hal yang tidak dilakukan perintah itu sendiri."
  rows:
    - case: "Ganti jaringan atau Wi-Fi, bangun dari mode tidur"
      ssh: "Mulai ulang sendiri"
      shprot: "Tersambung ulang otomatis"
    - case: "Terowongan tersambung tetapi tidak ada lalu lintas"
      ssh: "Tidak disadari"
      shprot: "Terdeteksi oleh pemeriksaan lewat proxy"
    - case: "Kegagalan autentikasi berulang"
      ssh: "Tergantung skrip Anda"
      shprot: "Percobaan jarang agar server tidak memblokir IP Anda"
    - case: "Kunci klien"
      ssh: "`ssh-keygen` dan berkas di `~/.ssh`"
      shprot: "Buat atau impor, salin kunci publik dengan satu klik"
    - case: "Pemeriksaan identitas server"
      ssh: "Pertanyaan di terminal"
      shprot: "Dialog dengan sidik jari kunci host"
    - case: "Aplikasi tanpa dukungan SOCKS5"
      ssh: "Tidak didukung"
      shprot: "Proxy HTTP opsional lewat terowongan yang sama"
    - case: "Status terowongan"
      ssh: "Jendela terminal"
      shprot: "Ikon baki dan notifikasi beserta penyebab kegagalan"
  note: "Alat seperti `autossh` membantu memulai ulang, tetapi tetap perlu disiapkan manual dan tidak punya antarmuka."

uses:
  title: "Cocok untuk apa"
  items:
    - id: infra
      title: "Infrastruktur Anda sendiri"
      text: "Akses dasbor internal, lingkungan staging, dan alat admin yang hanya bisa dibuka dari jaringan server Anda."
    - id: wifi
      title: "Jaringan yang tidak tepercaya"
      text: "Lewatkan lalu lintas melalui server sendiri saat memakai Wi-Fi publik di kafe, hotel, atau bandara."
    - id: laptop
      title: "Kerja jarak jauh dengan laptop"
      text: "Proxy yang stabil di komputer yang seharian tidur dan berganti jaringan."

how:
  title: "Siap dalam tiga langkah"
  steps:
    - title: "Siapkan server"
      text: "Server Linux apa pun dengan OpenSSH bisa dipakai. Tidak ada yang dipasang di sana — pengguna khusus yang hanya boleh meneruskan port dibuat dengan beberapa perintah."
    - title: "Sambungkan Shprot"
      text: "Masukkan `user@server`, buat kunci, dan tambahkan bagian publiknya ke `authorized_keys` di server. Periksa sidik jari server sekali saja."
    - title: "Arahkan aplikasi ke proxy"
      text: "Gunakan `socks5h://localhost:1080` di peramban atau pengaturan proxy sistem — menu baki menyalin alamatnya untuk Anda."
  guide: "Panduan langkah demi langkah dengan pengamanan server"

tour:
  title: "Lebih dekat"
  items:
    - id: trust
      caption: "Pada sambungan pertama Anda melihat sidik jari kunci server dan memutuskan apakah akan memercayainya."
    - id: tray
      caption: "Menu baki menampilkan status terowongan dan menyalin alamat proxy yang siap dipakai."

download:
  title: "Unduh Shprot"
  lead: "Versi {version}. Gratis, sumber terbuka, tanpa akun atau langganan."
  windows:
    title: "Windows"
    text: "Windows 10 dan 11, 64-bit. Untuk memperbarui, pasang versi baru di atas versi lama."
    button: "Pemasang (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+, dan distribusi yang kompatibel."
    button: "Paket (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+, dan distribusi yang kompatibel."
    button: "Paket (.rpm)"
  requirements: "Shprot memakai klien OpenSSH (`ssh` dan `ssh-keygen`). Klien ini sudah ada di Windows 10 / 11 dan sebagian besar desktop Linux; jika tidak ada, Shprot akan menjelaskan cara menambahkannya."
  verify: "Setiap rilis menyertakan berkas `SHA256SUMS` untuk memastikan unduhan tidak diubah."
  all_releases: "Semua rilis dan perubahan"

faq:
  title: "Tanya jawab"
  items:
    - q: "Apakah Shprot itu VPN?"
      a: "Bukan, ini proxy. Hanya aplikasi yang diatur untuk memakainya — peramban, `curl`, `git`, IDE — yang mengirim lalu lintas lewat terowongan; sisanya langsung. Tidak ada adaptor jaringan virtual yang dipasang."
    - q: "Apa yang saya butuhkan di server?"
      a: "Server OpenSSH biasa dan akun yang diizinkan meneruskan port TCP. Tidak ada yang perlu dipasang di server; VPS atau server rumahan mana pun yang bisa diakses lewat SSH bisa dipakai."
    - q: "Bisakah saya masuk dengan kata sandi?"
      a: "Tidak, Shprot hanya memakai kunci SSH. Kunci lebih aman daripada kata sandi dan memungkinkan terowongan tersambung ulang tanpa campur tangan Anda. Kunci dibuat di aplikasi dengan satu klik."
    - q: "Aplikasi apa saja yang bisa memakai proxy?"
      a: "Peramban, `curl`, `git`, pengelola paket, dan aplikasi apa pun yang punya pengaturan proxy SOCKS5 atau HTTP atau mendukung variabel `ALL_PROXY` / `HTTP_PROXY`. Gunakan `socks5h://` agar nama situs di-resolve di server."
    - q: "Bagaimana Shprot memeriksa terowongan?"
      a: "Sesekali ia membuka situs populer yang dipilih acak melalui proxy. Jika gagal padahal internet tersedia, terowongan dimulai ulang."
    - q: "Apakah gratis?"
      a: "Ya. Shprot adalah perangkat lunak bebas dan sumber terbuka berlisensi GNU GPL v3, tanpa akun, langganan, atau iklan."
    - q: "Apakah ada versi macOS?"
      a: "Belum. Saat ini Shprot mendukung Windows 10 / 11 dan Linux."

footer:
  license: "Shprot adalah perangkat lunak bebas berlisensi GNU GPL v3."
  source: "Kode sumber"
  issues: "Laporkan masalah"
---
