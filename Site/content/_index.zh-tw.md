---
title: "Shprot — 自動重新連線的 SSH SOCKS5 代理"
description: "適用於 Windows 與 Linux 的免費桌面應用程式，透過 SSH 維持 SOCKS5 與 HTTP 代理：網路切換與睡眠喚醒後自動重新連線，偵測卡住的通道，無需命令列。"
readme: "README.md"

ui:
  skip: "跳至主要內容"
  nav_features: "功能"
  nav_compare: "為什麼選 Shprot"
  nav_download: "下載"
  nav_faq: "常見問題"
  language: "語言"
  github: "GitHub 上的原始碼"

hero:
  title: "始終保持連線的 SSH SOCKS5 代理"
  lead: "Shprot 能把任何可以透過 SSH 連線的伺服器變成穩定的 SOCKS5 與 HTTP 代理。它持續監看網路，在切換 Wi-Fi 或從睡眠中喚醒後自動恢復通道，並清楚告訴你哪裡出了問題——完全不需要命令列。"
  download: "下載"
  download_hint: "{version} 版，支援 Windows 與 Linux"
  facts:
    - "免費開放原始碼"
    - "Windows 10 / 11 與 Linux"
    - "49 種語言"
  screenshot_alt: "Shprot 設定視窗"

features:
  title: "讓通道持續運作所需的一切"
  items:
    - id: reconnect
      title: "不怕網路變化"
      text: "切換 Wi-Fi、插上網路線或喚醒筆電——Shprot 都會察覺，並自動恢復通道。"
    - id: health
      title: "揪出無聲的故障"
      text: "透過代理本身定期檢查，找出看似已連線卻沒有流量通過的通道，並將其重新啟動。"
    - id: gentle
      title: "對伺服器友善"
      text: "重試間隔會逐漸拉長，登入失敗後最多每分鐘重試一次，因此不會因為金鑰錯誤而被 fail2ban 封鎖 IP。"
    - id: keys
      title: "兩下點擊搞定 SSH 金鑰"
      text: "直接在設定視窗中產生或匯入金鑰，並複製公開金鑰以加入 `authorized_keys`。"
    - id: trust
      title: "清楚的伺服器驗證"
      text: "首次連線時，伺服器主機金鑰的指紋會顯示在對話方塊中，讓你在信任伺服器之前先行核對。"
    - id: http
      title: "也支援 HTTP 代理"
      text: "不支援 SOCKS5 的應用程式可以使用選用的 HTTP 代理，它同樣經過這條通道。"
    - id: tray
      title: "常駐系統匣"
      text: "圖示顯示通道是否開啟，通知會說明未開啟的原因，記錄檔一鍵即可開啟。"
    - id: languages
      title: "說你的語言"
      text: "介面已翻譯成 49 種語言，阿拉伯文、波斯文、希伯來文與烏爾都文採用由右至左的版面。"

compare:
  title: "為什麼不直接用 `ssh -D`？"
  lead: "`ssh -D 1080 user@server` 一行指令就能得到 SOCKS5 代理——直到連線悄悄失效為止。Shprot 執行的是同一個 OpenSSH 用戶端，並處理好單一指令做不到的一切。"
  rows:
    - case: "網路變化、切換 Wi-Fi、從睡眠中喚醒"
      ssh: "需要自己重新啟動"
      shprot: "自動重新連線"
    - case: "通道已連線卻沒有流量"
      ssh: "無人察覺"
      shprot: "透過代理檢查即可發現"
    - case: "重複的驗證失敗"
      ssh: "取決於你的指令碼"
      shprot: "低頻重試，伺服器不會封鎖你的 IP"
    - case: "用戶端金鑰"
      ssh: "`ssh-keygen` 與 `~/.ssh` 中的檔案"
      shprot: "產生或匯入，一鍵複製公開金鑰"
    - case: "伺服器身分驗證"
      ssh: "終端機提示"
      shprot: "顯示主機金鑰指紋的對話方塊"
    - case: "不支援 SOCKS5 的應用程式"
      ssh: "不支援"
      shprot: "經同一通道的選用 HTTP 代理"
    - case: "通道狀態"
      ssh: "終端機視窗"
      shprot: "系統匣圖示與附上失敗原因的通知"
  note: "`autossh` 之類的工具能協助重新啟動，但仍需手動設定，也沒有使用者介面。"

uses:
  title: "適用情境"
  items:
    - id: infra
      title: "你自己的基礎架構"
      text: "存取只能從伺服器所在網路開啟的內部儀表板、測試環境與管理工具。"
    - id: wifi
      title: "不可信任的網路"
      text: "在咖啡廳、飯店或機場使用公共 Wi-Fi 時，讓流量經過你自己的伺服器。"
    - id: laptop
      title: "用筆電遠端工作"
      text: "在整天睡眠、頻繁切換網路的電腦上維持穩定的代理。"

how:
  title: "三個步驟即可開始"
  steps:
    - title: "準備伺服器"
      text: "任何安裝了 OpenSSH 的 Linux 伺服器都可以，不需要安裝任何軟體。只要幾行指令就能建立一個只允許連接埠轉送的專用使用者。"
    - title: "連線 Shprot"
      text: "輸入 `user@server`，產生金鑰並把公開金鑰加入伺服器的 `authorized_keys`。伺服器指紋只需核對一次。"
    - title: "讓應用程式使用代理"
      text: "在瀏覽器或系統代理設定中填入 `socks5h://localhost:1080`——系統匣選單可以幫你複製這個位址。"
  guide: "包含伺服器強化的逐步設定指南"

tour:
  title: "細節一覽"
  items:
    - id: trust
      caption: "首次連線時，你會看到伺服器的金鑰指紋，並決定是否信任它。"
    - id: tray
      caption: "系統匣選單顯示通道狀態，並可複製立即可用的代理位址。"

download:
  title: "下載 Shprot"
  lead: "{version} 版。免費開放原始碼，無需帳號，沒有訂閱。"
  windows:
    title: "Windows"
    text: "Windows 10 與 11，64 位元。更新時直接在舊版上安裝新版即可。"
    button: "安裝程式 (.exe)"
  deb:
    title: "Ubuntu、Debian"
    text: "Ubuntu 22.04+、Debian 12+ 及相容的發行版。"
    button: "套件 (.deb)"
  rpm:
    title: "Fedora、RHEL"
    text: "Fedora 39+、RHEL 9+、AlmaLinux 9+ 及相容的發行版。"
    button: "套件 (.rpm)"
  requirements: "Shprot 使用 OpenSSH 用戶端（`ssh` 與 `ssh-keygen`）。Windows 10 / 11 與大多數 Linux 桌面環境已內建；若缺少，Shprot 會告訴你如何安裝。"
  verify: "每個版本都附有 `SHA256SUMS` 檔案，可用來確認下載的檔案未經竄改。"
  all_releases: "所有版本與變更內容"

faq:
  title: "常見問題"
  items:
    - q: "Shprot 是 VPN 嗎？"
      a: "不是，它是代理。只有設定為使用它的應用程式（瀏覽器、`curl`、`git`、IDE 等）的流量會經過通道，其餘流量直接連線。不會安裝虛擬網路卡。"
    - q: "伺服器上需要什麼？"
      a: "一台普通的 OpenSSH 伺服器，以及允許 TCP 連接埠轉送的帳號。伺服器上不需要安裝任何軟體，任何能透過 SSH 連線的 VPS 或家用伺服器都可以。"
    - q: "可以用密碼登入嗎？"
      a: "不行，Shprot 只使用 SSH 金鑰。金鑰比密碼更安全，也能讓通道在無人操作時自動重新連線。在應用程式中一鍵即可產生金鑰。"
    - q: "哪些應用程式可以使用這個代理？"
      a: "瀏覽器、`curl`、`git`、套件管理員，以及任何具備 SOCKS5 或 HTTP 代理設定、或支援 `ALL_PROXY` / `HTTP_PROXY` 環境變數的應用程式。請使用 `socks5h://`，讓網站名稱在伺服器端解析。"
    - q: "Shprot 如何檢查通道？"
      a: "它會不時透過代理開啟一個隨機挑選的熱門網站。如果網際網路本身可用卻存取失敗，就會重新啟動通道。"
    - q: "免費嗎？"
      a: "是的。Shprot 是採用 GNU GPL v3 授權的自由開放原始碼軟體，無需帳號，沒有訂閱與廣告。"
    - q: "有 macOS 版本嗎？"
      a: "目前還沒有。Shprot 現階段支援 Windows 10 / 11 與 Linux。"

footer:
  license: "Shprot 是採用 GNU GPL v3 授權的自由軟體。"
  source: "原始碼"
  issues: "回報問題"
---
