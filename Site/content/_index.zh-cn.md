---
title: "Shprot — 自动重连的 SSH SOCKS5 代理"
description: "适用于 Windows 和 Linux 的免费桌面应用，通过 SSH 提供稳定的 SOCKS5 和 HTTP 代理：网络切换和睡眠唤醒后自动重连，发现卡住的隧道，无需命令行。"
readme: "README.zh.md"

ui:
  skip: "跳到正文"
  nav_features: "功能"
  nav_compare: "为什么选 Shprot"
  nav_download: "下载"
  nav_faq: "常见问题"
  language: "语言"
  github: "GitHub 上的源代码"

hero:
  title: "始终在线的 SSH SOCKS5 代理"
  lead: "Shprot 能把任何可以通过 SSH 访问的服务器变成稳定的 SOCKS5 和 HTTP 代理。它持续关注网络状态，在切换 Wi-Fi 或从睡眠中唤醒后自动恢复隧道，并清楚地告诉你哪里出了问题——全程无需命令行。"
  download: "下载"
  download_hint: "{version} 版，支持 Windows 和 Linux"
  facts:
    - "免费开源"
    - "Windows 10 / 11 和 Linux"
    - "49 种语言"
  screenshot_alt: "Shprot 设置窗口"

features:
  title: "让隧道持续运行所需的一切"
  items:
    - id: reconnect
      title: "网络变化也不怕"
      text: "切换 Wi-Fi、插上网线或唤醒笔记本——Shprot 都会察觉，并自动恢复隧道。"
    - id: health
      title: "发现无声的故障"
      text: "通过代理本身进行定期检查，发现看似已连接却不通流量的隧道，并将其重启。"
    - id: gentle
      title: "对服务器友好"
      text: "重试间隔逐渐增加，登录失败后最多每分钟重试一次，因此不会因为密钥错误而被 fail2ban 封禁 IP。"
    - id: keys
      title: "两下点击搞定 SSH 密钥"
      text: "直接在设置窗口中生成或导入密钥，并复制公钥以添加到 `authorized_keys`。"
    - id: trust
      title: "清晰的服务器验证"
      text: "首次连接时会在对话框中显示服务器主机密钥指纹，你可以先核对，再决定是否信任该服务器。"
    - id: http
      title: "还有 HTTP 代理"
      text: "不支持 SOCKS5 的应用可以使用可选的 HTTP 代理，它同样经过这条隧道。"
    - id: tray
      title: "常驻系统托盘"
      text: "图标显示隧道是否已开启，通知说明未开启的原因，日志一键即可查看。"
    - id: languages
      title: "说你的语言"
      text: "界面已翻译成 49 种语言，阿拉伯语、波斯语、希伯来语和乌尔都语采用从右到左布局。"

compare:
  title: "为什么不直接用 `ssh -D`？"
  lead: "`ssh -D 1080 user@server` 一条命令就能得到 SOCKS5 代理——直到连接悄无声息地失效。Shprot 运行的是同一个 OpenSSH 客户端，并处理好单条命令做不到的一切。"
  rows:
    - case: "网络变化、切换 Wi-Fi、从睡眠中唤醒"
      ssh: "需要自己重启"
      shprot: "自动重连"
    - case: "隧道已连接但不通流量"
      ssh: "无人察觉"
      shprot: "通过代理健康检查发现"
    - case: "反复认证失败"
      ssh: "取决于你的脚本"
      shprot: "低频重试，服务器不会封禁你的 IP"
    - case: "客户端密钥"
      ssh: "`ssh-keygen` 和 `~/.ssh` 中的文件"
      shprot: "生成或导入，一键复制公钥"
    - case: "服务器身份验证"
      ssh: "终端提示"
      shprot: "显示主机密钥指纹的对话框"
    - case: "不支持 SOCKS5 的应用"
      ssh: "不支持"
      shprot: "经同一隧道的可选 HTTP 代理"
    - case: "隧道状态"
      ssh: "终端窗口"
      shprot: "托盘图标和附带失败原因的通知"
  note: "`autossh` 之类的工具可以帮助重启，但仍需手动配置，也没有用户界面。"

uses:
  title: "适用场景"
  items:
    - id: infra
      title: "你自己的基础设施"
      text: "访问只能从服务器所在网络打开的内部面板、测试环境和管理工具。"
    - id: wifi
      title: "不可信的网络"
      text: "在咖啡馆、酒店或机场使用公共 Wi-Fi 时，让流量经过你自己的服务器。"
    - id: laptop
      title: "用笔记本远程办公"
      text: "在整天睡眠、频繁换网的电脑上保持稳定的代理。"

how:
  title: "三步即可上手"
  steps:
    - title: "准备服务器"
      text: "任何装有 OpenSSH 的 Linux 服务器都可以，无需安装任何软件。只需几条命令即可创建一个只允许端口转发的专用用户。"
    - title: "连接 Shprot"
      text: "输入 `user@server`，生成密钥并把公钥添加到服务器的 `authorized_keys`。核对一次服务器指纹即可。"
    - title: "让应用使用代理"
      text: "在浏览器或系统代理设置中填写 `socks5h://localhost:1080`——托盘菜单可以帮你复制这个地址。"
  guide: "包含服务器加固的分步设置指南"

tour:
  title: "细节一览"
  items:
    - id: trust
      caption: "首次连接时，你会看到服务器的密钥指纹，并决定是否信任它。"
    - id: tray
      caption: "托盘菜单显示隧道状态，并可复制现成的代理地址。"

download:
  title: "下载 Shprot"
  lead: "{version} 版。免费开源，无需账户，没有订阅。"
  windows:
    title: "Windows"
    text: "Windows 10 和 11，64 位。更新时直接在旧版本上安装新版本即可。"
    button: "安装程序 (.exe)"
  deb:
    title: "Ubuntu、Debian"
    text: "Ubuntu 22.04+、Debian 12+ 及兼容发行版。"
    button: "软件包 (.deb)"
  rpm:
    title: "Fedora、RHEL"
    text: "Fedora 39+、RHEL 9+、AlmaLinux 9+ 及兼容发行版。"
    button: "软件包 (.rpm)"
  requirements: "Shprot 使用 OpenSSH 客户端（`ssh` 和 `ssh-keygen`）。Windows 10 / 11 和大多数 Linux 桌面系统已自带；如果缺失，Shprot 会告诉你如何添加。"
  verify: "每个版本都附带 `SHA256SUMS` 文件，用于检查下载的文件是否被篡改。"
  all_releases: "所有版本及更新内容"

faq:
  title: "常见问题"
  items:
    - q: "Shprot 是 VPN 吗？"
      a: "不是，它是代理。只有设置为使用它的应用（浏览器、`curl`、`git`、IDE 等）的流量才会经过隧道，其余流量直接连接。不会安装虚拟网卡。"
    - q: "服务器上需要什么？"
      a: "一个普通的 OpenSSH 服务器，以及一个允许 TCP 端口转发的账户。服务器上无需安装任何软件，任何能通过 SSH 访问的 VPS 或家用服务器都可以。"
    - q: "可以用密码登录吗？"
      a: "不可以，Shprot 只使用 SSH 密钥。密钥比密码更安全，也能让隧道在无人值守时自动重连。在应用中一键即可生成密钥。"
    - q: "哪些应用可以使用这个代理？"
      a: "浏览器、`curl`、`git`、包管理器，以及任何带有 SOCKS5 或 HTTP 代理设置、或支持 `ALL_PROXY` / `HTTP_PROXY` 环境变量的应用。请使用 `socks5h://`，让网站域名在服务器端解析。"
    - q: "Shprot 如何检查隧道？"
      a: "它会不时通过代理打开一个随机选择的热门网站。如果互联网本身可用而访问失败，就会重启隧道。"
    - q: "免费吗？"
      a: "是的。Shprot 是基于 GNU GPL v3 许可证的自由开源软件，无需账户，没有订阅和广告。"
    - q: "有 macOS 版本吗？"
      a: "暂时没有。Shprot 目前支持 Windows 10 / 11 和 Linux。"

footer:
  license: "Shprot 是基于 GNU GPL v3 许可证的自由软件。"
  source: "源代码"
  issues: "报告问题"
---
