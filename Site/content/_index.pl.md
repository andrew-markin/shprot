---
title: "Shprot — proxy SOCKS5 przez SSH, które samo się ponownie łączy"
description: "Darmowa aplikacja na Windows i Linux, która utrzymuje działające proxy SOCKS5 i HTTP przez SSH: łączy się ponownie po zmianie sieci i uśpieniu, wykrywa zawieszone tunele, bez wiersza poleceń."
readme: "README.md"

ui:
  skip: "Przejdź do treści"
  nav_features: "Funkcje"
  nav_compare: "Dlaczego Shprot"
  nav_download: "Pobierz"
  nav_faq: "Pytania"
  language: "Język"
  github: "Kod źródłowy na GitHubie"

hero:
  title: "Proxy SOCKS5 przez SSH, które nie zrywa połączenia"
  lead: "Shprot zamienia każdy serwer, do którego masz dostęp przez SSH, w stabilne proxy SOCKS5 i HTTP. Pilnuje sieci, przywraca tunel po zmianie Wi-Fi i wybudzeniu z uśpienia oraz mówi dokładnie, co poszło nie tak — bez wiersza poleceń."
  download: "Pobierz"
  download_hint: "Wersja {version} dla Windows i Linux"
  facts:
    - "Darmowy i otwartoźródłowy"
    - "Windows 10 / 11 i Linux"
    - "49 języków"
  screenshot_alt: "Okno ustawień Shprot"

features:
  title: "Wszystko, czego tunel potrzebuje, by działać dalej"
  items:
    - id: reconnect
      title: "Przetrwa zmianę sieci"
      text: "Zmiana Wi-Fi, podłączony kabel czy laptop wybudzony z uśpienia — Shprot to zauważa i sam przywraca tunel."
    - id: health
      title: "Wyłapuje ciche awarie"
      text: "Regularne testy przez samo proxy znajdują tunel, który wygląda na połączony, ale nie przepuszcza ruchu, i uruchamiają go ponownie."
    - id: gentle
      title: "Oszczędza twój serwer"
      text: "Kolejne próby są stopniowo coraz rzadsze, a po nieudanych logowaniach powtarzane najwyżej raz na minutę, więc fail2ban nie zablokuje twojego IP przez zły klucz."
    - id: keys
      title: "Klucze SSH w dwa kliknięcia"
      text: "Wygeneruj lub zaimportuj klucz i skopiuj część publiczną do `authorized_keys` prosto z okna ustawień."
    - id: trust
      title: "Czytelna weryfikacja serwera"
      text: "Przy pierwszym połączeniu odcisk klucza serwera jest pokazywany w oknie dialogowym, aby można go było sprawdzić, zanim zaufasz serwerowi."
    - id: http
      title: "Także proxy HTTP"
      text: "Programy bez obsługi SOCKS5 mogą korzystać z opcjonalnego proxy HTTP, które działa przez ten sam tunel."
    - id: tray
      title: "Mieszka w zasobniku"
      text: "Ikona pokazuje, czy tunel działa, powiadomienia wyjaśniają, dlaczego nie, a dziennik jest o jedno kliknięcie."
    - id: languages
      title: "Mówi twoim językiem"
      text: "Interfejs przetłumaczono na 49 języków, z układem od prawej do lewej dla arabskiego, perskiego, hebrajskiego i urdu."

compare:
  title: "Dlaczego nie po prostu `ssh -D`?"
  lead: "`ssh -D 1080 user@server` daje proxy SOCKS5 jednym poleceniem — dopóki połączenie po cichu nie przestanie działać. Shprot uruchamia tego samego klienta OpenSSH i zajmuje się wszystkim, czego samo polecenie nie robi."
  rows:
    - case: "Zmiana sieci lub Wi-Fi, wybudzenie z uśpienia"
      ssh: "Uruchom ponownie sam"
      shprot: "Łączy się ponownie automatycznie"
    - case: "Tunel połączony, ale ruch nie przechodzi"
      ssh: "Pozostaje niezauważone"
      shprot: "Wykrywane przez testy przez proxy"
    - case: "Powtarzające się błędy uwierzytelniania"
      ssh: "Zależy od twoich skryptów"
      shprot: "Rzadkie próby, aby serwer nie zablokował twojego IP"
    - case: "Klucze klienta"
      ssh: "`ssh-keygen` i pliki w `~/.ssh`"
      shprot: "Wygeneruj lub zaimportuj, skopiuj klucz publiczny jednym kliknięciem"
    - case: "Weryfikacja tożsamości serwera"
      ssh: "Pytanie w terminalu"
      shprot: "Okno z odciskiem klucza serwera"
    - case: "Programy bez obsługi SOCKS5"
      ssh: "Brak wsparcia"
      shprot: "Opcjonalne proxy HTTP przez ten sam tunel"
    - case: "Stan tunelu"
      ssh: "Okno terminala"
      shprot: "Ikona w zasobniku i powiadomienia z przyczyną błędu"
  note: "Narzędzia takie jak `autossh` pomagają przy restartach, ale nadal wymagają ręcznej konfiguracji i nie mają interfejsu."

uses:
  title: "Do czego się przydaje"
  items:
    - id: infra
      title: "Własna infrastruktura"
      text: "Dostęp do wewnętrznych paneli, środowisk testowych i narzędzi administracyjnych dostępnych tylko z sieci twojego serwera."
    - id: wifi
      title: "Niezaufane sieci"
      text: "Kieruj ruch przez własny serwer, gdy korzystasz z publicznego Wi-Fi w kawiarniach, hotelach czy na lotniskach."
    - id: laptop
      title: "Praca zdalna na laptopie"
      text: "Stabilne proxy na komputerze, który przez cały dzień zasypia i zmienia sieci."

how:
  title: "Gotowe w trzech krokach"
  steps:
    - title: "Przygotuj serwer"
      text: "Wystarczy dowolny serwer Linux z OpenSSH. Nic się na nim nie instaluje — osobnego użytkownika, który może tylko przekierowywać porty, tworzy się kilkoma poleceniami."
    - title: "Połącz Shprot"
      text: "Wpisz `user@server`, wygeneruj klucz i dodaj jego część publiczną do `authorized_keys` na serwerze. Raz sprawdź odcisk serwera."
    - title: "Skieruj programy na proxy"
      text: "Użyj `socks5h://localhost:1080` w przeglądarce lub systemowych ustawieniach proxy — menu w zasobniku skopiuje adres za ciebie."
  guide: "Instrukcja krok po kroku z zabezpieczeniem serwera"

tour:
  title: "Z bliska"
  items:
    - id: trust
      caption: "Przy pierwszym połączeniu widzisz odcisk klucza serwera i decydujesz, czy mu zaufać."
    - id: tray
      caption: "Menu w zasobniku pokazuje stan tunelu i kopiuje gotowe adresy proxy."

download:
  title: "Pobierz Shprot"
  lead: "Wersja {version}. Darmowy, otwartoźródłowy, bez kont i subskrypcji."
  windows:
    title: "Windows"
    text: "Windows 10 i 11, 64-bitowy. Aby zaktualizować, zainstaluj nową wersję na starszą."
    button: "Instalator (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ i zgodne dystrybucje."
    button: "Pakiet (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ i zgodne dystrybucje."
    button: "Pakiet (.rpm)"
  requirements: "Shprot korzysta z klienta OpenSSH (`ssh` i `ssh-keygen`). Jest on dostępny w Windows 10 / 11 i większości środowisk Linux; jeśli go brakuje, Shprot podpowie, jak go dodać."
  verify: "Każde wydanie zawiera plik `SHA256SUMS`, który pozwala sprawdzić, czy pobrany plik nie został zmieniony."
  all_releases: "Wszystkie wydania i zmiany"

faq:
  title: "Pytania i odpowiedzi"
  items:
    - q: "Czy Shprot to VPN?"
      a: "Nie, to proxy. Przez tunel idzie ruch tylko programów skonfigurowanych do korzystania z niego — przeglądarki, `curl`, `git`, IDE; reszta idzie bezpośrednio. Żadne wirtualne karty sieciowe nie są instalowane."
    - q: "Czego potrzebuję na serwerze?"
      a: "Zwykłego serwera OpenSSH i konta z prawem do przekierowywania portów TCP. Na serwerze niczego nie trzeba instalować; wystarczy dowolny VPS lub serwer domowy dostępny przez SSH."
    - q: "Czy mogę logować się hasłem?"
      a: "Nie, Shprot używa tylko kluczy SSH. Klucze są bezpieczniejsze od haseł i pozwalają tunelowi łączyć się ponownie bez twojego udziału. Klucz generuje się w aplikacji jednym kliknięciem."
    - q: "Jakie programy mogą korzystać z proxy?"
      a: "Przeglądarki, `curl`, `git`, menedżery pakietów i każdy program z ustawieniami proxy SOCKS5 lub HTTP albo obsługą zmiennych `ALL_PROXY` / `HTTP_PROXY`. Używaj `socks5h://`, aby nazwy stron były rozwiązywane na serwerze."
    - q: "Jak Shprot sprawdza tunel?"
      a: "Co jakiś czas otwiera przez proxy losowo wybraną popularną stronę. Jeśli to się nie uda, choć internet jest dostępny, tunel jest uruchamiany ponownie."
    - q: "Czy to jest darmowe?"
      a: "Tak. Shprot to wolne i otwartoźródłowe oprogramowanie na licencji GNU GPL v3, bez kont, subskrypcji i reklam."
    - q: "Czy jest wersja na macOS?"
      a: "Jeszcze nie. Obecnie Shprot obsługuje Windows 10 / 11 i Linux."

footer:
  license: "Shprot to wolne oprogramowanie na licencji GNU GPL v3."
  source: "Kod źródłowy"
  issues: "Zgłoś problem"
---
