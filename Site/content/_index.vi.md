---
title: "Shprot — proxy SOCKS5 qua SSH tự động kết nối lại"
description: "Ứng dụng máy tính miễn phí cho Windows và Linux giúp duy trì proxy SOCKS5 và HTTP qua SSH: tự kết nối lại sau khi đổi mạng và ngủ, phát hiện đường hầm bị treo, không cần dòng lệnh."
readme: "README.md"

ui:
  skip: "Chuyển đến nội dung"
  nav_features: "Tính năng"
  nav_compare: "Vì sao chọn Shprot"
  nav_download: "Tải về"
  nav_faq: "Hỏi đáp"
  language: "Ngôn ngữ"
  github: "Mã nguồn trên GitHub"

hero:
  title: "Proxy SOCKS5 qua SSH luôn giữ kết nối"
  lead: "Shprot biến bất kỳ máy chủ nào bạn truy cập được qua SSH thành proxy SOCKS5 và HTTP ổn định. Ứng dụng theo dõi mạng, khôi phục đường hầm sau khi đổi Wi-Fi hoặc thức dậy từ chế độ ngủ, và cho bạn biết chính xác điều gì đã xảy ra — không cần dòng lệnh."
  download: "Tải về"
  download_hint: "Phiên bản {version} cho Windows và Linux"
  facts:
    - "Miễn phí và mã nguồn mở"
    - "Windows 10 / 11 và Linux"
    - "49 ngôn ngữ"
  screenshot_alt: "Cửa sổ cài đặt của Shprot"

features:
  title: "Mọi thứ một đường hầm cần để luôn hoạt động"
  items:
    - id: reconnect
      title: "Vững vàng khi đổi mạng"
      text: "Đổi Wi-Fi, cắm cáp mạng hay đánh thức laptop — Shprot nhận ra và tự khôi phục đường hầm."
    - id: health
      title: "Bắt được lỗi âm thầm"
      text: "Các lần kiểm tra định kỳ qua chính proxy phát hiện đường hầm trông như đã kết nối nhưng không cho lưu lượng đi qua, rồi khởi động lại nó."
    - id: gentle
      title: "Nhẹ nhàng với máy chủ"
      text: "Các lần thử lại thưa dần, và sau khi đăng nhập thất bại chỉ thử lại tối đa mỗi phút một lần, nên fail2ban sẽ không chặn IP của bạn vì một khóa sai."
    - id: keys
      title: "Khóa SSH chỉ với hai cú nhấp"
      text: "Tạo hoặc nhập khóa, rồi sao chép phần công khai cho `authorized_keys` ngay từ cửa sổ cài đặt."
    - id: trust
      title: "Xác minh máy chủ rõ ràng"
      text: "Ở lần kết nối đầu tiên, dấu vân tay khóa máy chủ hiện trong hộp thoại để bạn kiểm tra trước khi tin tưởng máy chủ."
    - id: http
      title: "Có cả proxy HTTP"
      text: "Ứng dụng không hỗ trợ SOCKS5 có thể dùng proxy HTTP tùy chọn đi qua cùng đường hầm."
    - id: tray
      title: "Nằm ở khay hệ thống"
      text: "Biểu tượng cho biết đường hầm có đang mở không, thông báo giải thích lý do nếu không, và nhật ký chỉ cách một cú nhấp."
    - id: languages
      title: "Nói ngôn ngữ của bạn"
      text: "Giao diện được dịch sang 49 ngôn ngữ, với bố cục từ phải sang trái cho tiếng Ả Rập, Ba Tư, Do Thái và Urdu."

compare:
  title: "Sao không chỉ dùng `ssh -D`?"
  lead: "`ssh -D 1080 user@server` cho bạn proxy SOCKS5 chỉ bằng một lệnh — cho đến khi kết nối âm thầm ngừng hoạt động. Shprot chạy cùng ứng dụng khách OpenSSH và lo mọi việc mà riêng lệnh đó không làm."
  rows:
    - case: "Đổi mạng, đổi Wi-Fi, thức dậy từ chế độ ngủ"
      ssh: "Tự khởi động lại"
      shprot: "Tự động kết nối lại"
    - case: "Đường hầm đã kết nối nhưng không có lưu lượng"
      ssh: "Không ai nhận ra"
      shprot: "Phát hiện bằng kiểm tra qua proxy"
    - case: "Lỗi xác thực lặp lại"
      ssh: "Tùy vào script của bạn"
      shprot: "Thử lại thưa thớt để máy chủ không chặn IP của bạn"
    - case: "Khóa phía máy khách"
      ssh: "`ssh-keygen` và các tệp trong `~/.ssh`"
      shprot: "Tạo hoặc nhập, sao chép khóa công khai bằng một cú nhấp"
    - case: "Xác minh danh tính máy chủ"
      ssh: "Câu hỏi trong terminal"
      shprot: "Hộp thoại với dấu vân tay khóa máy chủ"
    - case: "Ứng dụng không hỗ trợ SOCKS5"
      ssh: "Không hỗ trợ"
      shprot: "Proxy HTTP tùy chọn qua cùng đường hầm"
    - case: "Trạng thái đường hầm"
      ssh: "Cửa sổ terminal"
      shprot: "Biểu tượng khay và thông báo kèm nguyên nhân lỗi"
  note: "Các công cụ như `autossh` giúp khởi động lại, nhưng vẫn cần cấu hình thủ công và không có giao diện."

uses:
  title: "Dùng để làm gì"
  items:
    - id: infra
      title: "Hạ tầng của riêng bạn"
      text: "Truy cập bảng điều khiển nội bộ, môi trường thử nghiệm và công cụ quản trị chỉ mở từ mạng của máy chủ."
    - id: wifi
      title: "Mạng không đáng tin cậy"
      text: "Đưa lưu lượng qua máy chủ của riêng bạn khi dùng Wi-Fi công cộng ở quán cà phê, khách sạn hay sân bay."
    - id: laptop
      title: "Làm việc từ xa trên laptop"
      text: "Giữ proxy ổn định trên máy tính cả ngày ngủ rồi thức và đổi mạng liên tục."

how:
  title: "Sẵn sàng trong ba bước"
  steps:
    - title: "Chuẩn bị máy chủ"
      text: "Bất kỳ máy chủ Linux nào có OpenSSH đều dùng được. Không cần cài gì lên đó — một người dùng riêng chỉ được chuyển tiếp cổng có thể tạo bằng vài lệnh."
    - title: "Kết nối Shprot"
      text: "Nhập `user@server`, tạo khóa và thêm phần công khai vào `authorized_keys` trên máy chủ. Kiểm tra dấu vân tay máy chủ một lần."
    - title: "Trỏ ứng dụng tới proxy"
      text: "Dùng `socks5h://localhost:1080` trong trình duyệt hoặc cài đặt proxy của hệ thống — menu khay sẽ sao chép địa chỉ giúp bạn."
  guide: "Hướng dẫn cài đặt từng bước kèm gia cố máy chủ"

tour:
  title: "Xem kỹ hơn"
  items:
    - id: trust
      caption: "Ở lần kết nối đầu tiên, bạn thấy dấu vân tay khóa máy chủ và quyết định có tin tưởng hay không."
    - id: tray
      caption: "Menu khay hiển thị trạng thái đường hầm và sao chép địa chỉ proxy sẵn sàng sử dụng."

download:
  title: "Tải Shprot"
  lead: "Phiên bản {version}. Miễn phí, mã nguồn mở, không cần tài khoản hay gói đăng ký."
  windows:
    title: "Windows"
    text: "Windows 10 và 11, 64 bit. Để cập nhật, chỉ cần cài phiên bản mới đè lên bản cũ."
    button: "Trình cài đặt (.exe)"
  deb:
    title: "Ubuntu, Debian"
    text: "Ubuntu 22.04+, Debian 12+ và các bản phân phối tương thích."
    button: "Gói (.deb)"
  rpm:
    title: "Fedora, RHEL"
    text: "Fedora 39+, RHEL 9+, AlmaLinux 9+ và các bản phân phối tương thích."
    button: "Gói (.rpm)"
  requirements: "Shprot dùng ứng dụng khách OpenSSH (`ssh` và `ssh-keygen`). Nó có sẵn trong Windows 10 / 11 và hầu hết các môi trường Linux; nếu thiếu, Shprot sẽ hướng dẫn bạn cách thêm vào."
  verify: "Mỗi bản phát hành đều có tệp `SHA256SUMS` để kiểm tra tệp tải về không bị thay đổi."
  all_releases: "Tất cả bản phát hành và thay đổi"

faq:
  title: "Câu hỏi và trả lời"
  items:
    - q: "Shprot có phải là VPN không?"
      a: "Không, đây là proxy. Chỉ những ứng dụng được thiết lập để dùng nó — trình duyệt, `curl`, `git`, IDE — mới gửi lưu lượng qua đường hầm; mọi thứ khác đi trực tiếp. Không có bộ điều hợp mạng ảo nào được cài đặt."
    - q: "Tôi cần gì trên máy chủ?"
      a: "Một máy chủ OpenSSH thông thường và một tài khoản được phép chuyển tiếp cổng TCP. Không cần cài gì lên máy chủ; bất kỳ VPS hay máy chủ tại nhà nào truy cập được qua SSH đều dùng được."
    - q: "Tôi có thể đăng nhập bằng mật khẩu không?"
      a: "Không, Shprot chỉ dùng khóa SSH. Khóa an toàn hơn mật khẩu và cho phép đường hầm tự kết nối lại mà không cần bạn. Khóa được tạo trong ứng dụng chỉ bằng một cú nhấp."
    - q: "Những ứng dụng nào có thể dùng proxy?"
      a: "Trình duyệt, `curl`, `git`, trình quản lý gói và mọi ứng dụng có cài đặt proxy SOCKS5 hoặc HTTP hay hỗ trợ biến `ALL_PROXY` / `HTTP_PROXY`. Hãy dùng `socks5h://` để tên trang web được phân giải trên máy chủ."
    - q: "Shprot kiểm tra đường hầm như thế nào?"
      a: "Thỉnh thoảng nó mở một trang web phổ biến được chọn ngẫu nhiên qua proxy. Nếu việc này thất bại trong khi internet vẫn hoạt động, đường hầm sẽ được khởi động lại."
    - q: "Có miễn phí không?"
      a: "Có. Shprot là phần mềm tự do mã nguồn mở theo giấy phép GNU GPL v3, không tài khoản, không gói đăng ký, không quảng cáo."
    - q: "Có phiên bản cho macOS không?"
      a: "Chưa có. Hiện Shprot hỗ trợ Windows 10 / 11 và Linux."

footer:
  license: "Shprot là phần mềm tự do theo giấy phép GNU GPL v3."
  source: "Mã nguồn"
  issues: "Báo cáo sự cố"
---
