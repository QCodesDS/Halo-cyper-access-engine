# Brief — Halo Cyber Access Engine

> File này dành cho **người đọc**. Ngôn ngữ tự nhiên, không technical.

---

## Dự án này là gì?

Halo là một công cụ phân tích log truy cập mạng nội bộ doanh nghiệp,
được xây dựng bằng C++ thuần túy (không dùng thư viện cao cấp).

Bối cảnh: Một công ty công nghệ lớn bị sự cố an ninh mạng. Hệ thống
ghi nhận hàng triệu sự kiện từ người dùng, thiết bị, ứng dụng. Đội
ngũ bảo mật không kịp phân tích vì dữ liệu quá lớn và tìm kiếm quá chậm.

Halo ra đời để giải quyết bài toán đó.

---

## Halo làm được gì?

**Giai đoạn 1 (Midterm):** Nền tảng lưu trữ và tìm kiếm

- Tải file log CSV lên đến 10.000 dòng vào bộ nhớ
- Tìm kiếm nhanh theo người dùng, thiết bị, tài nguyên
- Truy vết hành trình của một người dùng: họ dùng thiết bị nào → ứng dụng nào → truy cập tài nguyên gì
- Xem ai đã truy cập vào một tài nguyên cụ thể
- Thống kê top 10 tài nguyên được truy cập nhiều nhất

**Giai đoạn 2 (Final):** Phát hiện hành vi bất thường

- Xử lý tập dữ liệu cực lớn: 1 triệu dòng trở lên
- Phát hiện các dấu hiệu nguy hiểm như: đăng nhập thất bại liên tục,
  một thiết bị đột ngột truy cập quá nhiều tài nguyên, người dùng
  xuất hiện ở nhiều quốc gia trong thời gian không thể di chuyển được,
  phiên làm việc kéo dài bất thường, chuỗi thao tác admin + download liên tiếp...

---

## Ai sẽ dùng Halo?

Analyst bảo mật (SOC team) — những người cần nhanh chóng tra cứu
và phát hiện điểm bất thường trong biển dữ liệu log khổng lồ.

---

## Vì sao project này thú vị?

Đây là bài toán thực tế trong ngành cybersecurity. Thay vì dùng
thư viện có sẵn, mình phải tự xây dựng hash table, dynamic array,
sorted index từ đầu — giống như viết một mini database engine.

Đây là cơ hội hiếm để hiểu sâu cách dữ liệu thực sự được tổ chức
và tìm kiếm ở tầng thấp nhất.

---

## Phạm vi của đồ án

- **Trong scope:** CLI app C++, xử lý CSV, query engine, anomaly detection
- **Ngoài scope:** Giao diện đồ họa, network thực, database thực, web API
- **Deadline midterm:** ~2 tuần từ ngày nhận đề
- **Deadline final:** ~3 tuần sau midterm
