# Hướng Dẫn Sử Dụng: Halo – Cyber Access Engine

Tài liệu này cung cấp hướng dẫn chi tiết cách biên dịch, cài đặt và vận hành hệ thống phát hiện hành vi bất thường mạng **Halo Cyber Access Engine**.

---

## 1. Yêu Cầu Môi Trường & Hệ Thống

Để biên dịch và chạy chương trình độc lập, máy tính của bạn cần đáp ứng các điều kiện tối thiểu sau:
- **Hệ điều hành**: Windows (10, 11), Linux (Ubuntu 20.04+), hoặc macOS.
- **Trình biên dịch**: 
  - GCC / g++ hỗ trợ tiêu chuẩn **C++17** trở lên.
  - Hoặc Clang / clang++ tương đương.
  - Hoặc MSVC (Visual Studio 2019 trở lên).
- **Công cụ Build**: **CMake** phiên bản từ **3.16** trở lên.

---

## 2. Hướng Dẫn Biên Dịch Hệ Thống

### 2.1. Biên dịch chế độ Release (Tối ưu hóa tốc độ - Dành cho vận hành thực tế)
Biên dịch chế độ Release kích hoạt tối ưu hóa mã nguồn cấp cao `-O3` của trình biên dịch, đảm bảo tốc độ tối đa khi tải và xử lý hàng triệu dòng dữ liệu.

```bash
# 1. Tạo và cấu hình thư mục build ở chế độ Release
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Thực hiện biên dịch
cmake --build build
```
*Tệp khả thi độc lập sẽ được xuất ra thư mục:* `release/bin/halo` (hoặc `release/bin/halo.exe` trên Windows).

### 2.2. Biên dịch chế độ Debug (Phát triển & Gỡ lỗi)
Biên dịch chế độ Debug giữ lại thông tin gỡ lỗi đầy đủ và không tối ưu hóa mã nguồn, hỗ trợ dò lỗi bộ nhớ hoặc đứt gãy bằng các công cụ gỡ lỗi (gdb, lldb).

```bash
# 1. Tạo và cấu hình thư mục build ở chế độ Debug
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# 2. Thực hiện biên dịch
cmake --build build
```
*Tệp khả thi debug sẽ được xuất ra thư mục:* `debug/bin/halo` (hoặc `debug/bin/halo.exe` trên Windows).

---

## 3. Vận Hành CLI & Ví Dụ Cụ Thể

Hãy khởi động ứng dụng từ terminal để bắt đầu tương tác CLI:
```bash
./release/bin/halo
```

Giao diện sẽ hiển thị lời chào:
```text
=== Halo Cyber Access Engine ===
Type 'help' for available commands
> 
```

Dưới đây là danh sách các lệnh CLI đầy đủ, định dạng đầu ra và ví dụ thực nghiệm cụ thể:

### Lệnh 1: Nạp tệp tin dữ liệu (`load <filepath>`)
- **Mô tả**: Tải dữ liệu log mạng từ tệp tin CSV, xác thực định dạng từng bản ghi, thực hiện loại bỏ trùng lặp và dựng chỉ mục đa chiều (Hash & Sorted) để sẵn sàng cho các truy vấn.
- **Cú pháp**: `load <đường_dẫn_file_csv>`
- **Ví dụ**:
  ```text
  > load src/data/halo_dataset_1k.csv
  ```
- **Định dạng Output mẫu**:
  ```text
  [INFO] Loaded 1000 records (0 skipped, 0 duplicates removed)
  [INFO] Indexes built in 0.01s
  ```

### Lệnh 2: Truy vấn Hành trình Người dùng (`query user <uid> <t_start> <t_end>`)
- **Mô tả**: Liệt kê chi tiết toàn bộ chuỗi hành vi truy cập của một người dùng trong khoảng thời gian xác định, được sắp xếp tăng dần theo dòng thời gian.
- **Cú pháp**: `query user <user_id> <epoch_bắt_đầu> <epoch_kết_thúc>`
- **Ví dụ**:
  ```text
  > query user U090 1715000000 1716000000
  ```
- **Định dạng Output mẫu**:
  ```text
  ===============================================================================
                       USER JOURNEY FOR USER: U090
  ===============================================================================
  [2024-05-06 14:02:40] D396 → APP102 → R071 (ACCESS @ US)
  [2024-05-06 14:24:20] D396 → APP102 → R071 (ACCESS @ US)
  [2024-05-06 20:30:10] D405 → APP103 → R025 (FAILED_LOGIN @ VN)
  ===============================================================================
  Total matching events: 3
  ```

### Lệnh 3: Truy vấn Hành trình Tài nguyên (`query resource <rid> <t_start> <t_end>`)
- **Mô tả**: Liệt kê chi tiết lịch sử truy cập của các tài khoản khác nhau tới một tài nguyên cụ thể trong khoảng thời gian trích xuất.
- **Cú pháp**: `query resource <resource_id> <epoch_bắt_đầu> <epoch_kết_thúc>`
- **Ví dụ**:
  ```text
  > query resource R048 1715000000 1716000000
  ```
- **Định dạng Output mẫu**:
  ```text
  ===============================================================================
                    RESOURCE JOURNEY FOR RESOURCE: R048
  ===============================================================================
  [2024-05-06 12:10:00] U017 → D101 → APP202 (ACCESS @ VN)
  [2024-05-06 13:45:00] U032 → D105 → APP202 (ACCESS @ VN)
  ===============================================================================
  Total matching events: 2
  ```

### Lệnh 4: Thống kê Tài nguyên Tần suất cao (`top resources <t_start> <t_end>`)
- **Mô tả**: Tổng hợp và hiển thị danh sách các tài nguyên được truy cập nhiều nhất trong khoảng thời gian yêu cầu, được sắp xếp giảm dần theo lượt truy cập.
- **Cú pháp**: `top resources <epoch_bắt_đầu> <epoch_kết_thúc>`
- **Ví dụ**:
  ```text
  > top resources 1715000000 1716000000
  ```
- **Định dạng Output mẫu**:
  ```text
  RANK    RESOURCE ID     COUNT
  ───────────────────────────────────────
  1       R025            84
  2       R071            52
  3       R048            21
  4       R012            12
  ```

### Lệnh 5: Phát hiện hành vi bất thường toàn diện (`detect anomaly`)
- **Mô tả**: Quét và chấm điểm tất cả 14 hành vi bất thường (từ A01 đến A14) dựa trên cấu hình ngưỡng chuẩn BRD. Hiển thị báo cáo thống kê trực quan và Top 10 đối tượng đáng ngờ nhất.
- **Cú pháp**: `detect anomaly`
- **Ví dụ**:
  ```text
  > detect anomaly
  ```
- **Định dạng Output mẫu**:
  ```text
  [INFO] Running threshold detectors...
  [INFO] Running behavior detectors...
  [INFO] Building sessions...
  [INFO] Running session detectors...
  [INFO] Running advanced detectors...
  [INFO] Sorting 1043 anomalies...
  [INFO] Deduping anomalies...

  ===============================================================================
                        ANOMALY DETECTION REPORT
  ===============================================================================
  Total anomalies found: 1012

  ANOMALY SUMMARY BY TYPE
  ───────────────────────────────────────────────────────────────────────────────
  TYPE                     COUNT   RISK
  ───────────────────────────────────────────────────────────────────────────────
  A04_OFF_HOURS            592     LOW
  A05_IMPOSSIBLE_TRAVEL    49      CRITICAL
  A14_LATERAL_MOVEMENT     371     HIGH

  TOP 10 SUSPICIOUS USERS
  ───────────────────────────────────────────────────────────────────────────────
  RANK    USER ID     ANOMALY COUNT
  ───────────────────────────────────────────────────────────────────────────────
  1       U090        21
  2       U017        18
  3       U018        18
  ===============================================================================
  [INFO] Detection completed in 0.0047s
  ```

### Lệnh 6: Phát hiện hành vi bất thường lọc theo loại (`detect anomaly --type=...`)
- **Mô tả**: Chỉ quét và báo cáo các hành vi bất thường thuộc nhóm chức năng được yêu cầu để tối ưu hóa thời gian phân tích và lọc nhiễu thông tin.
- **Cú pháp**: 
  - `detect anomaly --type=threshold` (Chỉ dò A01 - A04)
  - `detect anomaly --type=behavior` (Chỉ dò A05 - A06)
  - `detect anomaly --type=session` (Chỉ dò A07 - A09)
  - `detect anomaly --type=advanced` (Chỉ dò A10 - A14)
- **Ví dụ**:
  ```text
  > detect anomaly --type=behavior
  ```

### Lệnh 7: Trợ giúp lệnh CLI (`help`)
- **Mô tả**: Liệt kê đầy đủ danh sách các lệnh khả dụng và định dạng nhập liệu chuẩn.
- **Ví dụ**:
  ```text
  > help
  ```

### Lệnh 8: Thoát chương trình (`exit`)
- **Mô tả**: Thu hồi triệt để mọi tài nguyên động (mảng, bảng băm, đối tượng cảnh báo, phiên hoạt động) để bảo vệ rò rỉ bộ nhớ, sau đó kết thúc chương trình.
- **Ví dụ**:
  ```text
  > exit
  ```
- **Định dạng Output mẫu**:
  ```text
  [INFO] Memory freed. Goodbye.
  ```

---

## 4. Xử Lý Các Lỗi Thường Gặp (Troubleshooting)

### Lỗi 1: `[ERROR] No data loaded. Use 'load <filepath>' first.`
- **Hiện tượng**: Gõ các lệnh `query` hoặc `detect` nhưng chương trình báo lỗi và không xuất kết quả.
- **Nguyên nhân**: Bạn chưa thực hiện nạp dữ liệu log bằng lệnh `load` trước đó, hoặc việc nạp tệp dữ liệu trước đó bị lỗi thất bại.
- **Cách khắc phục**: Hãy chạy lệnh nạp dữ liệu hợp lệ đầu tiên, ví dụ: `load src/data/halo_dataset_1k.csv`, và đợi thông báo thành công từ hệ thống.

### Lỗi 2: `[ERROR] Failed to open file: <đường_dẫn>`
- **Hiện tượng**: Không thể nạp tệp tin CSV dù đã chạy lệnh `load`.
- **Nguyên nhân**:
  - Đường dẫn tệp tin cung cấp bị sai chính tả hoặc không tồn tại.
  - Chương trình không có quyền đọc (read permission) đối với tệp dữ liệu đó.
- **Cách khắc phục**: Hãy kiểm tra kỹ sự tồn tại của tệp tin dữ liệu bằng trình quản lý tệp tin hoặc lệnh `ls/dir` trên terminal. Đảm bảo đường dẫn tuyệt đối hoặc tương đối là chính xác tuyệt đối.

### Lỗi 3: Lỗi sai định dạng tham số lệnh (Ví dụ: `Unknown argument: ...`)
- **Hiện tượng**: Nhập lệnh CLI nhưng nhận thông báo cảnh báo tham số không được nhận diện.
- **Nguyên nhân**:
  - Viết sai số lượng tham số cần thiết (Ví dụ: lệnh `query user` thiếu mốc thời gian bắt đầu hoặc kết thúc).
  - Định dạng thời gian Unix Epoch bị sai ký tự (phải là số nguyên dương).
  - Viết sai cú pháp cờ lọc loại bất thường (Ví dụ: gõ sai `--type=threshold` thành `-type=threshold`).
- **Cách khắc phục**: Gõ lệnh `help` để đối chiếu định dạng chuẩn của từng lệnh. Điều chỉnh tham số cho đúng định dạng số nguyên và chạy lại.
