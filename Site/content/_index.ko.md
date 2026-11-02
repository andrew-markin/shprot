---
title: "Shprot — 스스로 다시 연결되는 SSH 기반 SOCKS5 프록시"
description: "SSH를 통한 SOCKS5 및 HTTP 프록시를 유지해 주는 Windows·Linux용 무료 데스크톱 앱. 네트워크 변경과 절전 후 다시 연결하고 멈춘 터널을 감지합니다. 명령줄이 필요 없습니다."
readme: "README.md"

ui:
  skip: "본문으로 건너뛰기"
  nav_features: "기능"
  nav_compare: "왜 Shprot인가"
  nav_download: "다운로드"
  nav_faq: "FAQ"
  language: "언어"
  github: "GitHub 소스 코드"

hero:
  title: "연결이 끊기지 않는 SSH 기반 SOCKS5 프록시"
  lead: "Shprot은 SSH로 접속할 수 있는 모든 서버를 안정적인 SOCKS5 및 HTTP 프록시로 바꿔 줍니다. 네트워크를 지켜보다가 Wi-Fi 변경이나 절전 모드 해제 후 터널을 복구하고, 무엇이 잘못됐는지 정확히 알려 줍니다. 명령줄은 필요 없습니다."
  download: "다운로드"
  download_hint: "Windows 및 Linux용 버전 {version}"
  facts:
    - "무료 오픈 소스"
    - "Windows 10 / 11 및 Linux"
    - "49개 언어"
  screenshot_alt: "Shprot 설정 창"

features:
  title: "터널이 계속 작동하는 데 필요한 모든 것"
  items:
    - id: reconnect
      title: "네트워크 변화에도 끄떡없음"
      text: "Wi-Fi를 바꾸거나 케이블을 꽂거나 노트북을 절전 모드에서 깨워도 Shprot이 알아차리고 터널을 스스로 복구합니다."
    - id: health
      title: "조용한 장애도 포착"
      text: "프록시 자체를 통한 정기 점검으로, 연결된 것처럼 보이지만 트래픽이 흐르지 않는 터널을 찾아 다시 시작합니다."
    - id: gentle
      title: "서버를 배려하는 재시도"
      text: "재시도 간격은 점점 늘어나고, 로그인 실패 후에는 1분에 한 번 이하로만 재시도하므로 잘못된 키 때문에 fail2ban이 IP를 차단하지 않습니다."
    - id: keys
      title: "두 번의 클릭으로 SSH 키"
      text: "키를 생성하거나 가져오고, `authorized_keys`에 넣을 공개 키를 설정 창에서 바로 복사하세요."
    - id: trust
      title: "명확한 서버 확인"
      text: "처음 연결할 때 서버 호스트 키의 지문이 대화 상자에 표시되므로, 서버를 신뢰하기 전에 확인할 수 있습니다."
    - id: http
      title: "HTTP 프록시도 지원"
      text: "SOCKS5를 지원하지 않는 앱은 같은 터널을 통과하는 선택적 HTTP 프록시를 사용할 수 있습니다."
    - id: tray
      title: "트레이에 상주"
      text: "아이콘으로 터널 상태를 보여 주고, 알림으로 연결되지 않은 이유를 설명하며, 로그는 클릭 한 번이면 볼 수 있습니다."
    - id: languages
      title: "당신의 언어로"
      text: "인터페이스는 49개 언어로 번역되어 있으며, 아랍어·페르시아어·히브리어·우르두어는 오른쪽에서 왼쪽으로 배치됩니다."

compare:
  title: "그냥 `ssh -D`를 쓰면 안 될까요?"
  lead: "`ssh -D 1080 user@server` 명령 하나면 SOCKS5 프록시가 생깁니다. 연결이 소리 없이 멈추기 전까지는요. Shprot은 같은 OpenSSH 클라이언트를 실행하면서 명령 하나로는 할 수 없는 모든 일을 맡아 줍니다."
  rows:
    - case: "네트워크·Wi-Fi 변경, 절전 모드 해제"
      ssh: "직접 다시 실행"
      shprot: "자동으로 다시 연결"
    - case: "연결됐지만 트래픽이 흐르지 않는 터널"
      ssh: "알아차리지 못함"
      shprot: "프록시를 통한 점검으로 감지"
    - case: "반복되는 인증 실패"
      ssh: "스크립트에 달림"
      shprot: "드문 재시도로 서버가 IP를 차단하지 않음"
    - case: "클라이언트 키"
      ssh: "`ssh-keygen`과 `~/.ssh`의 파일"
      shprot: "생성 또는 가져오기, 공개 키를 클릭 한 번으로 복사"
    - case: "서버 신원 확인"
      ssh: "터미널 질문"
      shprot: "호스트 키 지문을 보여 주는 대화 상자"
    - case: "SOCKS5를 지원하지 않는 앱"
      ssh: "지원 안 됨"
      shprot: "같은 터널을 통한 선택적 HTTP 프록시"
    - case: "터널 상태"
      ssh: "터미널 창"
      shprot: "트레이 아이콘과 실패 이유가 담긴 알림"
  note: "`autossh` 같은 도구는 재시작을 도와주지만 여전히 수동 설정이 필요하고 사용자 인터페이스가 없습니다."

uses:
  title: "이럴 때 유용합니다"
  items:
    - id: infra
      title: "나만의 인프라"
      text: "서버 네트워크에서만 접근할 수 있는 내부 대시보드, 스테이징 환경, 관리 도구에 접속하세요."
    - id: wifi
      title: "신뢰할 수 없는 네트워크"
      text: "카페, 호텔, 공항의 공용 Wi-Fi를 쓸 때 트래픽을 내 서버를 통해 보내세요."
    - id: laptop
      title: "노트북으로 원격 근무"
      text: "하루 종일 절전 모드에 들어가고 네트워크를 바꾸는 컴퓨터에서도 안정적인 프록시를 유지하세요."

how:
  title: "세 단계로 시작"
  steps:
    - title: "서버 준비"
      text: "OpenSSH가 있는 Linux 서버라면 무엇이든 됩니다. 아무것도 설치하지 않으며, 포트 포워딩만 허용된 전용 사용자는 명령 몇 개로 만들 수 있습니다."
    - title: "Shprot 연결"
      text: "`user@server`를 입력하고 키를 생성한 뒤 공개 키를 서버의 `authorized_keys`에 추가합니다. 서버 지문은 한 번만 확인하면 됩니다."
    - title: "앱을 프록시로 연결"
      text: "브라우저나 시스템 프록시 설정에 `socks5h://localhost:1080`을 입력하세요. 트레이 메뉴에서 주소를 복사할 수 있습니다."
  guide: "서버 보안 강화를 포함한 단계별 설정 가이드"

tour:
  title: "자세히 보기"
  items:
    - id: trust
      caption: "처음 연결할 때 서버 키 지문을 보고 신뢰할지 결정합니다."
    - id: tray
      caption: "트레이 메뉴는 터널 상태를 보여 주고 바로 쓸 수 있는 프록시 주소를 복사합니다."

download:
  title: "Shprot 다운로드"
  lead: "버전 {version}. 무료 오픈 소스이며 계정이나 구독이 필요 없습니다."
  windows:
    title: "Windows"
    text: "Windows 10 및 11, 64비트. 업데이트하려면 새 버전을 기존 버전 위에 설치하면 됩니다."
    button: "설치 프로그램 (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04 이상, Debian 12 이상 및 호환 배포판."
    button: "패키지 (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39 이상, RHEL 9 이상, AlmaLinux 9 이상 및 호환 배포판."
    button: "패키지 (.rpm)"
  requirements: "Shprot은 OpenSSH 클라이언트(`ssh`와 `ssh-keygen`)를 사용합니다. Windows 10 / 11과 대부분의 Linux 데스크톱에 기본 포함되어 있으며, 없으면 Shprot이 추가 방법을 알려 줍니다."
  verify: "모든 릴리스에는 다운로드한 파일이 변조되지 않았는지 확인할 수 있는 `SHA256SUMS` 파일이 포함되어 있습니다."
  all_releases: "모든 릴리스와 변경 사항"

faq:
  title: "자주 묻는 질문"
  items:
    - q: "Shprot은 VPN인가요?"
      a: "아니요, 프록시입니다. 프록시를 사용하도록 설정한 앱(브라우저, `curl`, `git`, IDE 등)의 트래픽만 터널을 통과하고 나머지는 직접 연결됩니다. 가상 네트워크 어댑터는 설치되지 않습니다."
    - q: "서버에는 무엇이 필요한가요?"
      a: "일반 OpenSSH 서버와 TCP 포트 포워딩이 허용된 계정만 있으면 됩니다. 서버에 설치할 것은 없으며, SSH로 접속할 수 있는 VPS나 홈 서버라면 무엇이든 됩니다."
    - q: "비밀번호로 로그인할 수 있나요?"
      a: "아니요, Shprot은 SSH 키만 사용합니다. 키는 비밀번호보다 안전하고 터널이 사람 손 없이 다시 연결되게 해 줍니다. 키는 앱에서 클릭 한 번으로 생성됩니다."
    - q: "어떤 앱이 프록시를 사용할 수 있나요?"
      a: "브라우저, `curl`, `git`, 패키지 관리자, 그리고 SOCKS5나 HTTP 프록시 설정이 있거나 `ALL_PROXY` / `HTTP_PROXY` 변수를 지원하는 모든 앱입니다. 사이트 이름을 서버에서 확인하려면 `socks5h://`를 사용하세요."
    - q: "Shprot은 터널을 어떻게 점검하나요?"
      a: "가끔 무작위로 고른 인기 웹사이트를 프록시를 통해 엽니다. 인터넷은 되는데 이 요청이 실패하면 터널을 다시 시작합니다."
    - q: "무료인가요?"
      a: "네. Shprot은 GNU GPL v3 라이선스의 자유 오픈 소스 소프트웨어이며, 계정·구독·광고가 없습니다."
    - q: "macOS 버전이 있나요?"
      a: "아직 없습니다. 현재 Shprot은 Windows 10 / 11과 Linux를 지원합니다."

footer:
  license: "Shprot은 GNU GPL v3 라이선스의 자유 소프트웨어입니다."
  source: "소스 코드"
  issues: "문제 신고"
---
