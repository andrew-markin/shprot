<h1><img src="Assets/Shprot.svg" alt="Shprot Logo" width="30"> Shprot</h1>

🌐 **语言:** [English](README.md) | [Русский](README.ru.md) | [**简体中文**](README.zh.md)

🏠 **网站:** [shprot.mayfleet.com](https://shprot.mayfleet.com/zh-cn/)

**Shprot** 是一款桌面应用程序，为您提供可靠、自动恢复的 SOCKS5 代理（通过 SSH），无需使用命令行。

与标准的 SSH 隧道工具不同，它会监控您的网络，自动重启断开的连接，甚至能在您切换 Wi-Fi 或电脑从睡眠状态唤醒时自动适应。内置 SSH 密钥生成和灵活的代理设置，使配置变得简单且安全。

无论您是开发人员、测试人员，还是只需要一个稳定的代理进行日常浏览的用户，Shprot 都能以最少的精力保持您的连接畅通。

## 功能特性

- **自动化 SSH 隧道：** 轻松建立和管理安全的 SSH 隧道，通过本地 SOCKS5 代理路由流量。
- **智能连接恢复：** 自动监控互联网状态和网络切换（例如切换 Wi-Fi 网络或从睡眠中唤醒），并透明地重启隧道。
- **高级健康检查：** 定期通过代理测试热门网站的可用性来验证连接完整性，如果底层 SSH 进程停滞，则立即恢复隧道。
- **对服务器友好的重连机制：** 重复的重启尝试会以逐渐增加的间隔进行，认证失败后的重试频率不超过每分钟一次（间隔最长可增加到 30 分钟），因此配置错误的密钥不会导致您的 IP 地址被 fail2ban 等工具封禁。托盘菜单会显示距下次尝试的剩余时间，点击 **立即重新连接** 可跳过等待。
- **内置密钥管理：** 通过内置的 SSH 密钥生成简化客户端认证，让您轻松将公钥复制到远程服务器的 `authorized_keys` 文件中。
- **友好的身份验证：** 提供清晰的界面来管理远程服务器凭证和服务器/客户端认证参数。
- **可配置的网络设置：** 完全控制本地绑定地址和 SOCKS5 代理服务器的端口设置。
- **可选的 HTTP 代理支持：** 除 SOCKS5 代理外，您还可以启用独立的 HTTP 代理，通过相同的 SSH 隧道路由流量。这对于仅支持 HTTP 代理的旧应用程序或脚本非常有用。
- **多语言界面：** 界面已翻译为 49 种语言，并跟随系统界面语言；阿拉伯语、波斯语、希伯来语和乌尔都语使用从右到左的布局。

## 为什么不直接用 `ssh -D`？

`ssh -D 1080 user@server` 一条命令就能提供 SOCKS5 代理——直到连接悄无声息地失效。Shprot 运行的是同一个 OpenSSH 客户端，并负责处理这条命令本身无法处理的一切：

| | `ssh -D` | Shprot |
|---|---|---|
| 网络变化、切换 Wi-Fi、从睡眠中唤醒 | 需要手动重启 | 自动重新连接 |
| 隧道已连接但流量无法通过 | 无法察觉 | 通过代理进行的健康检查可以发现 |
| 反复认证失败 | 取决于您自己的脚本 | 低频重试，避免服务器封禁您的 IP |
| 客户端密钥 | `ssh-keygen` 和 `~/.ssh` 中的文件 | 生成或导入，一键复制公钥 |
| 服务器身份验证 | 终端提示 | 显示主机密钥指纹的对话框 |
| 不支持 SOCKS5 的应用 | 不支持 | 可选的 HTTP 代理，通过同一隧道 |
| 隧道状态 | 终端窗口 | 托盘图标以及包含失败原因的通知 |

`autossh` 等工具可以帮助自动重启，但仍需手动配置，并且没有用户界面。

**典型用途：**

- **自己的基础设施：** 访问只能从服务器所在网络访问的内部面板、测试环境和管理工具。
- **不受信任的网络：** 在咖啡馆、酒店或机场使用公共 Wi-Fi 时，让流量经过您自己的服务器。
- **使用笔记本电脑远程办公：** 在整天休眠和切换网络的电脑上保持稳定的代理。

## 系统要求

Shprot 适用于 **Windows** 和 **Linux** 平台：

- **Windows：** Windows 10 / 11（64 位）
- **Linux（基于 DEB）：** Ubuntu 22.04 或更新版本，Debian 12 或更新版本，以及其他兼容发行版
- **Linux（基于 RPM）：** Fedora 39 或更新版本，RHEL 9 / AlmaLinux 9 或更新版本
- **所有平台：** OpenSSH 客户端（`ssh` 和 `ssh-keygen`）。Windows 上通常已预装；如果没有，请在 Windows 设置的 *可选功能* 中添加 **"OpenSSH Client"**。Linux 上请安装 `openssh-client`（Ubuntu/Debian）或 `openssh-clients`（Fedora/RHEL）软件包。如果缺少客户端，Shprot 会给出提示。

## 安装

Shprot 适用于 Windows 和 Linux 平台。请从 **[Releases](../../releases)** 部分为您的操作系统选择适当的软件包。

### Windows
下载适用于 Windows 的预编译 NSIS 安装程序（`.exe` 文件）。只需运行下载的文件并按照安装向导说明操作即可。

更新时，只需在已安装版本上直接安装新版本：正在运行的应用程序会被关闭，旧版本会被替换，*应用和功能* 中只会保留一个 Shprot 条目（1.5 及更早版本遗留的条目也会被清理）。通过 *应用和功能* 卸载时，也会移除开机自启动项和开始菜单快捷方式。如需无人值守安装，请使用 `/S` 参数运行安装程序。

### Linux（基于 DEB 的发行版）
对于 Ubuntu、Debian 和其他基于 DEB 的系统，请下载 `.deb` 软件包并使用以下命令安装：

    sudo dpkg -i shprot_*.deb
    sudo apt-get install -f  # 解决依赖问题

该软件包在 Ubuntu 22.04 上构建，应适用于所有更新版本的 Ubuntu、Debian 12+ 和兼容发行版。

### Linux（基于 RPM 的发行版）
对于 Fedora、RHEL、AlmaLinux 和其他基于 RPM 的系统，请下载 `.rpm` 软件包并使用以下命令安装：

    sudo rpm -i shprot-*.rpm

或在 Fedora/RHEL 上使用 DNF：

    sudo dnf install ./shprot-*.rpm

该软件包在 Fedora 39 上构建，应适用于 Fedora 39+、RHEL 9+、AlmaLinux 9+ 和兼容发行版。

### 验证下载的文件
每个版本都附带一个 `SHA256SUMS` 文件，其中包含所有软件包的校验和。将其下载到软件包所在的文件夹，并确认软件包未被篡改：

- **Linux：**

      sha256sum -c SHA256SUMS --ignore-missing

- **Windows（PowerShell）：** 将输出与 `SHA256SUMS` 中对应文件的行进行比较：

      Get-FileHash .\Shprot-*.exe -Algorithm SHA256

## 服务端配置（安全加固）

为确保最大安全性，强烈建议在远程 Linux 服务器（例如 Ubuntu/Debian）上创建一个专用的受限用户，专门用于 SSH 隧道。这可以防止隧道账户访问命令 shell 或在服务器上执行任意脚本。

请按照以下步骤配置您的服务器：

### 1. 创建受限用户
运行以下命令创建一个名为 `tunnel` 的系统用户，无登录 shell 且锁定主目录：

    sudo useradd -r -s /bin/false -m tunnel

### 2. 配置 SSH 守护进程
使用 root 权限打开 SSH 配置文件：

    sudo nano /etc/ssh/sshd_config

滚动到文件底部并添加以下 `Match User` 块。这将明确隔离 `tunnel` 用户，禁用交互式终端分配，并仅允许 TCP 转发：

    Match User tunnel
        AllowTcpForwarding yes
        X11Forwarding no
        PermitTTY no
        ForceCommand echo "This account is for SSH tunneling only."

*保存文件并重启 SSH 服务以应用更改：`sudo systemctl restart ssh`。*

### 3. 添加 Shprot 公钥
现在，将 Shprot 生成的公钥安装到新用户的环境中。切换到 root 或使用 `sudo` 来设置 `authorized_keys` 文件：

    # 创建 .ssh 目录并设置正确的权限
    sudo mkdir -p /home/tunnel/.ssh
    sudo chmod 700 /home/tunnel/.ssh

    # 添加您的 Shprot 公钥
    echo "您的-Shprot-公钥" | sudo tee -a /home/tunnel/.ssh/authorized_keys

    # 修正所有权和权限
    sudo chmod 600 /home/tunnel/.ssh/authorized_keys
    sudo chown -R tunnel:tunnel /home/tunnel/

配置完成后，在 Shprot 桌面应用程序中设置连接时，使用 `tunnel` 作为 **SSH 用户名**（如果您的服务器不使用标准端口，请将 Port 字段保留为 `22`）。

## 应用程序配置

Shprot 完全独立于系统的全局 SSH 配置或现有的用户密钥，仅依赖其内部设置以实现更强的隔离和安全性。

<div align="center">
    <picture>
        <source media="(prefers-color-scheme: dark)" srcset="Assets/Screenshot-Dark.zh.png">
        <source media="(prefers-color-scheme: light)" srcset="Assets/Screenshot-Light.zh.png">
        <img alt="Shprot Interface Overview" src="Assets/Screenshot-Light.zh.png" width="754" height="779">
    </picture>
</div>

请按照以下步骤配置代理隧道：

### 步骤 1：启用隧道
1. 勾选 **"启用 SSH 代理隧道"** 复选框。
   * *注意：在勾选此复选框之前，所有其他配置控件将被禁用。您可以随时取消勾选以暂时禁用隧道，而不会丢失设置。*

### 步骤 2：配置远程服务器
1. 在 **"SSH 目标"** 字段中，使用格式输入连接详细信息：`username@host`（例如 `tunnel@192.0.2.1`）。
2. 在 **"端口"** 字段中，指定 SSH 连接的端口号。默认值为 `22`，这是标准的 SSH 端口。仅当您的服务器使用非标准端口时才需要更改此设置。
3. *（可选）* 点击下方的 **"运行信任检查"** 按钮。这将测试 Shprot 是否已经信任该主机密钥。您可以在服务器重新安装或怀疑网络异常时使用此按钮进行手动故障排除。

### 步骤 3：设置客户端密钥认证
1. 转到 **"SSH 私钥"** 区域。要提供安全的客户端密钥，您可以：
   * 将现有的私钥文本粘贴到字段中。换行符和末尾缺失的换行会自动修复。
   * 点击 **"导入…"** 从文件加载密钥。
   * 点击 **"生成"**（强烈推荐）让 Shprot 立即生成全新的加密密钥对。
2. 点击 **"复制公钥"** 将生成的公钥文本复制到剪贴板。
3. 打开服务器终端，按照服务器设置部分所示将此密钥附加到受限用户的文件中。

### 步骤 4：配置本地 SOCKS5 代理
1. 查看代理路由参数：
   * **本地 SOCKS5 代理地址：** 默认为 `localhost`（仅限本地计算机访问）。如果要与 LAN 中的其他设备共享代理，请将其更改为 `0.0.0.0`。
   * **端口：** 默认为 `1080`。如果系统中 `1080` 已被其他服务占用，您可以更改此端口。

> [!WARNING]
> **安全提示：** 代理没有身份验证。使用 `0.0.0.0`（或任何非回环地址）时，任何能够访问此端口的人都可以使用您的 SSH 隧道。保存此类地址时，Shprot 会要求您确认。请仅在受信任的网络中共享代理，并考虑使用防火墙限制访问。

### 步骤 5：可选的本地 HTTP 代理（高级）
如果您需要除 SOCKS5 之外的 HTTP 代理，请勾选 **"启用本地 HTTP 代理"** 复选框。这将解锁以下两个控件：

- **本地 HTTP 代理地址：** 默认设置为 `localhost`（与 SOCKS5 相同）。更改为 `0.0.0.0` 以与 LAN 中的其他设备共享 HTTP 代理。
- **端口：** 默认为 `8080`。如果此端口已被使用，您可以更改它。

步骤 4 中的安全提示同样适用于 HTTP 代理：它也没有身份验证。

HTTP 代理通过 **相同的** SSH 隧道工作，因此不需要任何额外的服务端配置。启用后，两个代理将同时可用。

### 步骤 6：保存并建立连接
1. 点击 **"保存"** 按钮。Shprot 将自动验证所有字段。如果发现任何错误，应用程序将高亮显示无效字段并指导您修复。
2. **主机验证（首次连接）：**
   如果您是首次连接到此服务器，将出现一个对话框，显示远程服务器的 **主机密钥指纹**。
   * **技术提示（如何验证）：** 为防止中间人攻击，您可以直接在服务器控制台上运行 `ssh-keygen -lf /etc/ssh/ssh_host_ed25519_key.pub`（或指定具体的主机密钥文件）来验证此指纹，并匹配输出哈希值。

   <div align="center">
       <picture>
           <source media="(prefers-color-scheme: dark)" srcset="Assets/HostVerification-Dark.zh.png">
           <source media="(prefers-color-scheme: light)" srcset="Assets/HostVerification-Light.zh.png">
           <img alt="“未知的服务器身份”对话框" src="Assets/HostVerification-Light.zh.png" width="578" height="271">
       </picture>
   </div>

3. 在对话框中确认信任。Shprot 将保存您的参数并立即启动后台引擎以打开和维护隧道。

### 连接状态指示器
您无需打开主窗口即可轻松跟踪隧道状态：
* **系统托盘图标：** 鲜艳的 **彩色图标** 表示隧道已激活且 SOCKS5 代理正在成功服务流量。**灰度图标** 表示隧道已关闭或断开。
* **托盘图标提示：** 将鼠标悬停在托盘图标上，即可查看隧道的当前状态；如果隧道已关闭，还会显示原因（例如认证失败或服务器不可达）。
* **通知消息：** 每当隧道成功连接或断开时，应用程序将发送本机系统托盘通知。如果无法打开隧道，通知会说明原因；每个新的原因只提示一次，而不是在每次重连尝试时都提示。

### 托盘菜单
右键单击托盘图标即可打开菜单，各项分组如下：
* **隧道状态** 位于顶部，例如 *隧道已打开* 或 *身份验证失败* 这样的简短原因及距下次重连尝试的剩余时间（完整说明见提示、通知和日志）。在不显示托盘提示的环境（例如 GNOME）中很有用。
* **立即重新连接** 会马上尝试连接，而不必等待下一次尝试。此项仅在等待期间显示。
* **启用 SSH 代理隧道** 无需打开设置即可开启或关闭隧道（与设置窗口中的复选框相同）。
* **复制 SOCKS5 代理地址** 和 **复制 HTTP 代理地址** 复制可直接使用的代理 URL，例如 `socks5h://localhost:1080` 或 `http://localhost:8080`，可用于浏览器、`curl` 或 `ALL_PROXY`/`HTTP_PROXY` 环境变量。仅当相应代理可用时才会显示。
* **首选项…** 打开设置窗口（双击图标也可以）。
* **显示通知** 开启或关闭通知消息。
* **登录时启动** 为当前用户开启或关闭登录时自动启动。
* **显示日志…** 显示最近的日志记录，并实时更新。
* **关于 Shprot…** 显示版本和构建信息，以及 Qt 及其第三方组件的许可证。
* **有新版本可用 (1.6 → 1.7)** 仅在 GitHub 上发布了新版本时出现，点击会打开该版本的页面。应用在启动一分钟后检查一次，之后每 12 小时检查一次；隧道打开时通过隧道检查。找到的版本会被记住，因此重启后该菜单项会立即显示。
* **退出 Shprot** 关闭隧道并退出应用程序。

<div align="center">
    <picture>
        <source media="(prefers-color-scheme: dark)" srcset="Assets/TrayMenu-Dark.zh.png">
        <source media="(prefers-color-scheme: light)" srcset="Assets/TrayMenu-Light.zh.png">
        <img alt="托盘菜单" src="Assets/TrayMenu-Light.zh.png" width="366" height="543">
    </picture>
</div>

### 故障排除
如果隧道无法打开：
1. 将鼠标悬停在托盘图标上，或查看最近的通知：其中会显示原因，例如认证失败或服务器不可达。
2. 在托盘菜单中选择 **显示日志…** 查看详细信息，包括 `ssh` 客户端的输出。

日志文件在 Windows 上保存在 `%APPDATA%\Shprot\Logs`，在 Linux 上保存在 `~/.local/share/Shprot/Logs`，每天一个文件，保留 30 天。

## 构建与开发

### 1. 环境准备

在构建或打开项目之前，请确保您的环境配置满足以下要求：

- **操作系统：** Windows 10/11（64 位）或 Linux（Ubuntu 22.04+、Fedora 39+）
- **编译器：**
  - Windows：MSVC 2022（x64）
  - Linux：GCC 11 或更新版本（或 Clang 14+）
- **框架：** Qt 6.11.2
- **构建系统：** CMake（版本 3.20 或更高）
- **构建生成器：** Ninja（强烈推荐以加快编译速度）或 MSBuild
- **安装包生成器：**
  - Windows：NSIS
  - Linux：使用 `fakeroot`/`dpkg-deb`（用于 DEB）和 `rpmbuild`（用于 RPM）的自定义脚本

按照以下步骤克隆仓库：

    git clone https://github.com/andrew-markin/shprot.git
    cd shprot

### 选项 A：构建发布包

项目使用 `.env` 文件进行环境变量配置。复制示例环境文件：

    cmake -E copy .env.example .env

打开新创建的 `.env` 文件，设置 `QT_PREFIX` 变量以指向本地 Qt 6.11.2 安装路径：

- **Windows（MSVC）：**

      QT_PREFIX=C:\Users\YourUsername\Qt\6.11.2\msvc2022_64

- **Linux（仅限本地构建）：**

      QT_PREFIX=/home/yourusername/Qt/6.11.2/gcc_64

> **注意：** `.env` 文件主要用于 Windows 构建。对于 Linux，建议使用基于 Docker 的构建（见下文），这不需要本地 Qt 安装。

#### Windows 构建
从项目根目录运行自动化发布脚本：

    ReleaseWindows.cmd

脚本执行完成后，您可以在 **`Release`** 文件夹中找到可用于生产的 NSIS 安装程序（`.exe`）。如果构建失败，脚本会停止并保留 `Release\Build` 文件夹以便排查（安装程序生成错误位于 `_CPack_Packages\win64\NSIS\NSISOutput.log`）。

#### Linux 构建（推荐：基于 Docker）

Linux 软件包的构建使用 Docker 以确保一致性和可重复性。这种方法创建干净、隔离的构建环境，并预配置了所有必要的依赖项。

**先决条件：** 在开发机器上安装 [Docker](https://docs.docker.com/engine/install/)。

**可用构建脚本：**

1. **适用于基于 DEB 的发行版（Ubuntu 22.04+）：**

       ./ReleaseUbuntu.sh

使用 `dpkg-deb` 生成 `.deb` 软件包，兼容 Ubuntu 22.04 及更新版本、Debian 12 及更新版本。

2. **适用于基于 RPM 的发行版（Fedora 39+）：**

       ./ReleaseFedora.sh

使用 `rpmbuild` 生成 `.rpm` 软件包，兼容 Fedora 39 及更新版本、RHEL 9 / AlmaLinux 9 及更新版本。

**关于 Docker 构建的重要说明：**

- **首次构建耗时较长：** 首次运行任一脚本通常需要 **10 到 40 分钟**（取决于 CPU 和网络速度），因为它会构建 Docker 镜像并从源代码编译轻量级、定制版的 Qt 6.11.2。这是一次性设置。
- **后续构建很快：** Docker 镜像在本地缓存后，所有后续构建将显著加快（通常只需几分钟）。
- **不需要本地 Qt：** 由于 Qt 捆绑在 Docker 容器中，您无需配置 `.env` 或在主机系统上安装 Qt。
- **输出位置：** 所有生成的软件包（`.deb` 或 `.rpm`）将出现在 **`Release`** 文件夹中，与 Windows 安装程序相同。
- **更新构建镜像：** 脚本仅在 Docker 镜像不存在时才会构建它。修改 `Dockerfile.ubuntu` 或 `Dockerfile.fedora` 后，请删除旧镜像（`docker rmi shprot-ubuntu-builder` 或 `docker rmi shprot-fedora-builder`），以便下次运行时重新构建。
- **Qt 源代码镜像：** 默认从 Yandex 镜像下载 Qt 源代码。如需使用其他镜像，请手动构建镜像，例如：

      docker build --build-arg QT_MIRROR=https://download.qt.io -t shprot-ubuntu-builder - < Dockerfile.ubuntu

#### Linux 构建（替代方案：本地构建）

如果您希望在不使用 Docker 的情况下进行本地构建，可以通过安装所需的构建依赖项并配置 `.env` 中的本地 Qt 路径来实现。

**所需软件包：**

- **Ubuntu/Debian：**

      sudo apt-get install build-essential cmake fakeroot dpkg-dev file ninja-build patchelf

- **Fedora/RHEL：**

      sudo dnf install cmake file gcc-c++ make ninja-build patchelf rpm-build

安装依赖项并在 `.env` 中设置 `QT_PREFIX` 后，您可以直接使用 CMake 进行构建。但是，**强烈推荐使用基于 Docker 的构建**，因为它能生成更一致、更可移植的软件包。

### 选项 B：代码开发

如果您想修改代码、修复错误或添加功能，应使用 **Qt Creator**：

1. 打开 **Qt Creator**。
2. 选择 **Open Project** 并导航到 **`Sources`** 目录。
3. 打开 `Sources` 文件夹中的 **`CMakeLists.txt`** 文件。
4. 当提示配置项目时，选择在 IDE 中配置的本地 **Qt 6.11.2** 套件。
   - *提示：为获得更快的构建速度，我们强烈建议在套件设置中使用 **Ninja** 作为 CMake 生成器，而不是默认生成器。*
5. 将 **构建目录** 设置为指向项目根目录中的 **`Builds`** 文件夹（例如 `../Builds/build-Shprot-Desktop-Debug`）。

对于 Linux 开发，您可以使用相同的基于 Docker 的方法或在系统上本地安装 Qt 6.11.2。

现在您可以直接在 IDE 中编写代码、调试和运行应用程序。中间构建文件将安全地保存在 `Builds` 文件夹中。

### 运行测试

单元测试（Qt Test）覆盖了工具函数、HTTP 代理以及检查更新时的版本比较。默认情况下不会构建测试；可通过 CMake 选项 `SHPROT_BUILD_TESTS` 启用：

    cmake -S Sources -B Builds/Tests -DCMAKE_PREFIX_PATH=<Qt 6.11.2 的路径> -DSHPROT_BUILD_TESTS=ON
    cmake --build Builds/Tests --target ShprotTests --parallel
    ctest --test-dir Builds/Tests --output-on-failure

在 Windows 上使用 Visual Studio 生成器时，请在构建命令中添加 `--config Release`，并在 `ctest` 命令中添加 `-C Release`。

### 翻译

翻译以 Qt Linguist `.ts` 文件的形式保存在 `Sources/Translations` 中，并编译进应用程序。语言列表由 `Sources/CMakeLists.txt` 中的 `TRANSLATION_LANGUAGES` 设置。修改代码中的文本后，请更新所有 `.ts` 文件并翻译新文本（例如使用 Qt Linguist）：

    cmake --build Builds/Tests --target update_translations

应用程序显示的 Qt 自身文本（标准按钮、文本框的上下文菜单）列在 `Sources/Translations.cpp` 中，并在同一组文件中翻译，因为不随附 Qt 的翻译。

### README 截图

`Assets` 中的截图在装有 Ubuntu GNOME 桌面的 VirtualBox 虚拟机中自动生成：

    ./TakeScreenshots.sh

该脚本会在虚拟机中直接运行 `Release` 文件夹中最新的 `.deb` 软件包（或通过参数指定的软件包），无需安装，按每个 README 的语言分别在浅色和深色主题下截取设置窗口、服务器密钥验证对话框和托盘菜单，并更新所有 README 文件中的图片尺寸。虚拟机的一次性准备步骤见脚本开头的说明。

### 自动构建与发布

仓库包含一个 GitHub Actions 工作流（`.github/workflows/build.yml`），会在每次推送到 `main` 以及每个 pull request 时构建 Windows 安装程序以及 DEB 和 RPM 软件包。同一工作流还会在所有三个平台上运行单元测试，生成的软件包可在工作流的 artifacts 中获取。

发布新版本的步骤：

1. 在 `Sources/CMakeLists.txt` 中更新版本号（`project(Shprot VERSION X.Y ...)`）。
2. 创建并推送对应的标签，例如 `git tag v1.5 && git push origin v1.5`。如果标签与项目版本不一致，工作流会立即停止。
3. 所有构建成功后，Releases 页面上会出现一个 **草稿** 版本，其中包含三个软件包和自动生成的发布说明。检查后请手动发布。

Docker 构建镜像缓存在 GitHub Container Registry（`ghcr.io`）中，仅在 Dockerfile 发生变化时才会重新构建，因此只有首次运行以及修改 Dockerfile 后的运行耗时较长。

### 网站

网站 [shprot.mayfleet.com](https://shprot.mayfleet.com/) 使用 [Hugo](https://gohugo.io/) 从 `Site` 文件夹构建，并在 `Site` 或 `Assets` 发生变化以及每次发布新版本时由 `.github/workflows/site.yml` 工作流发布到 GitHub Pages，因此下载链接始终指向最新版本。每种语言的页面文本保存在 `Site/content/_index.<语言>.md` 中；如果某个翻译缺少英文页面中的文本，构建会失败。在本地预览网站：

    hugo server --source Site

## 参与贡献

我们欢迎对 Shprot 的贡献！开始步骤：

1. **Fork** 仓库并本地克隆您的 fork。
2. 为您的更改创建一个 **新分支**（`git checkout -b feature/amazing-feature`）。
3. 确保您的环境按照上述开发部分所述正确配置。
4. 提交您的更改（`git commit -m '添加某个很棒的功能'`）并将其推送到您的 fork（`git push origin feature/amazing-feature`）。
5. 向我们的主仓库提交 **Pull Request**。

*在开始任何重大工作之前，请先创建一个 [Issue](../../issues) 与维护者讨论您的想法。*

## 许可证

本项目根据 **GNU GPL v3** 许可证条款发布。

完整许可证文本请参阅仓库根目录中的 [LICENSE](LICENSE) 文件。您可以根据 GPL v3 中概述的条件自由分发和/或修改此软件。
