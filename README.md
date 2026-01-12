# OpenKey - Phiên bản Phím tắt Nâng cao (Image Macro)

Đây là phiên bản tùy chỉnh được phát triển dựa trên bộ gõ tiếng Việt mã nguồn mở **OpenKey**. Phiên bản này tập trung nâng cấp mạnh mẽ hệ thống **Gõ tắt (Macro)**, cho phép người dùng gửi hình ảnh nhanh chóng thông qua từ khóa.

## 🔗 Nguồn gốc dự án
Dự án gốc được phát triển bởi tác giả **Mai Vu Tuyen**.
*   **Repository gốc:** [https://github.com/tuyenvm/OpenKey](https://github.com/tuyenvm/OpenKey)

## ✨ Tính năng mới: Gõ tắt Hình ảnh

Khác với các bộ gõ thông thường chỉ hỗ trợ thay thế văn bản, phiên bản này mang đến khả năng **"Gõ chữ ra Ảnh"**:

*   **Macro Hình ảnh:** Gán một từ khóa (ví dụ: `/hi`, `haha`) để gửi ngay một hình ảnh (meme, sticker, ảnh mẫu...) vào khung chat.
*   **Hỗ trợ Đa hình ảnh:** Cho phép chọn **nhiều ảnh** cho cùng một từ khóa. Khi gõ, toàn bộ các ảnh đã chọn sẽ được gửi đi cùng lúc.
*   **Giao diện Quản lý mới:** Cập nhật bảng quản lý Macro, hiển thị rõ ràng số lượng ảnh đính kèm cho mỗi phím tắt.
*   **Lưu trữ thông minh:** Hỗ trợ Xuất/Nhập dữ liệu macro bao gồm cả đường dẫn ảnh (định dạng tương thích mở rộng).

## ⚙️ Cơ chế hoạt động (Technical Mechanism)

Để thực hiện việc chèn ảnh vào các ứng dụng chat (Zalo, Messenger, Telegram, Word...) một cách tự nhiên nhất, OpenKey sử dụng cơ chế thao tác module **Clipboard** và **Input Simulation**:

1.  **Phát hiện từ khóa:** Khi người dùng gõ đúng phím tắt đã cài đặt.
2.  **Backup Clipboard:** Phần mềm tự động **sao lưu** nội dung văn bản hiện có trong Clipboard của người dùng (để tránh làm mất dữ liệu bạn đang copy).
3.  **Load dữ liệu ảnh:** OpenKey đọc file ảnh từ đường dẫn đã lưu và nạp dữ liệu ảnh đó vào Clipboard hệ thống.
4.  **Gửi lệnh Dán:** Phần mềm giả lập thao tác nhấn tổ hợp phím `Ctrl + V` để dán ảnh vào ứng dụng hiện tại.
5.  **Restore Clipboard:** Sau khi dán xong, phần mềm **khôi phục** lại nội dung văn bản cũ vào Clipboard.

> **Lưu ý:** Do sử dụng cơ chế `Ctrl + V`, quá trình dán ảnh diễn ra ngay lập tức và tương thích với hầu hết các phần mềm cho phép dán ảnh trên Windows.

## 🚀 Hướng dẫn nhanh
1.  Mở menu **Gõ tắt**.
2.  Nhập phím tắt và nội dung text (nếu muốn).
3.  Nhấn nút **"Thêm ảnh"** và chọn một hoặc nhiều file ảnh từ máy tính.
4.  Nhấn **"Lưu"**.
5.  Gõ phím tắt tại bất kỳ đâu để sử dụng.