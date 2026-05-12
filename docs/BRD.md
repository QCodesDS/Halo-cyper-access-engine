# BRD — Halo Cyber Access Engine

# Business Requirements Document v1.0

---

## 1. Chân dung người dùng

**Persona: Security Analyst (SOC)**

- Làm việc với terminal / CLI hằng ngày
- Cần tra cứu nhanh khi có sự cố, không có thời gian chờ
- Hiểu biết về timestamp, IP, event type
- Không cần UI đẹp, cần kết quả đúng và nhanh

---

## 2. Input Data Schema

**File:** CSV, dòng đầu là header

| Cột | Tên         | Kiểu   | Giá trị hợp lệ                                                                       |
| --- | ----------- | ------ | ------------------------------------------------------------------------------------ |
| 1   | user_id     | string | Ux (x là số)                                                                         |
| 2   | device_id   | string | Dx (x là số)                                                                         |
| 3   | app_id      | string | APPxxx                                                                               |
| 4   | resource_id | string | Rxxx                                                                                 |
| 5   | event_type  | enum   | LOGIN, LOGOUT, TOKEN_REFRESH, ACCESS, FAILED_LOGIN, OPEN_APP, DOWNLOAD, ADMIN_ACTION |
| 6   | location    | enum   | US, VN, JP, KR, SG, CN, DE, FR, UK, AU, CA, IN, BR, RU, TH                           |
| 7   | timestamp   | int64  | Unix epoch, > 0                                                                      |

**Ví dụ:**

```
user_id,device_id,app_id,resource_id,event_type,location,timestamp
U007,D018,APP003,R025,DOWNLOAD,SG,1713225863
U005,D025,APP018,R018,OPEN_APP,CN,1713108845
```

---

## 3. Functional Requirements

### FR-01: Load dữ liệu

- **Midterm:** Tối đa 10.000 dòng
- **Final:** Tối thiểu 1.000.000 dòng
- Bỏ qua dòng header
- Skip dòng có số cột ≠ 7
- Skip dòng có timestamp ≤ 0 hoặc không phải số
- Skip dòng có event_type không hợp lệ
- Loại bỏ dòng trùng lặp hoàn toàn (tất cả 7 field giống nhau)

### FR-02: Tiền xử lý

- Sắp xếp toàn bộ dữ liệu theo `timestamp` (tăng dần)
- Xây dựng hash index theo: `user_id`, `device_id`, `resource_id`
- Xây dựng session boundary cho mỗi user

### FR-03: Query — User Journey

```
Input:  user_id, timestamp_start, timestamp_end
Output: Danh sách các event trong khoảng thời gian, format:
        [timestamp] device_id → app_id → resource_id (event_type, location)
Sắp xếp: theo timestamp tăng dần
```

### FR-04: Query — Resource Journey

```
Input:  resource_id, timestamp_start, timestamp_end
Output: Danh sách các event trong khoảng thời gian, format:
        [timestamp] user_id → device_id → app_id (event_type, location)
Sắp xếp: theo timestamp tăng dần
```

### FR-05: Top 10 Resources

```
Input:  timestamp_start, timestamp_end
Output: Top 10 resource_id được truy cập nhiều nhất, kèm số lần
        Rank | resource_id | count
Sắp xếp: count giảm dần
```

### FR-06: Anomaly — Threshold Based

| ID  | Tên                      | Điều kiện mặc định                                    |
| --- | ------------------------ | ----------------------------------------------------- |
| A01 | Multi-device login       | User dùng > 3 device khác nhau trong 60 phút          |
| A02 | Consecutive failed login | User có > 5 FAILED_LOGIN liên tiếp                    |
| A03 | Resource flood           | Device truy cập > 20 resource khác nhau trong 30 phút |
| A04 | Off-hours access         | Event xảy ra ngoài 08:00–18:00 UTC                    |

### FR-07: Anomaly — Behavior Based

| ID  | Tên               | Điều kiện                                             |
| --- | ----------------- | ----------------------------------------------------- |
| A05 | Impossible travel | User xuất hiện ở 2+ quốc gia khác nhau trong 120 phút |
| A06 | Location churning | User đổi location > 3 lần trong 60 phút               |

### FR-08: Anomaly — Session Based

| ID  | Tên                | Điều kiện                                                          |
| --- | ------------------ | ------------------------------------------------------------------ |
| A07 | Long session       | Phiên > 8 tiếng                                                    |
| A08 | Session flood      | User tạo > 5 phiên trong 60 phút                                   |
| A09 | Dangerous sequence | Trong 1 phiên: có ADMIN_ACTION theo sau bởi DOWNLOAD trong 10 phút |

### FR-09: Anomaly — Advanced (Bonus)

| ID  | Tên                | Điều kiện                                                                         |
| --- | ------------------ | --------------------------------------------------------------------------------- |
| A10 | Brute force        | ≥ 5 FAILED_LOGIN liên tiếp, cuối cùng là LOGIN thành công                         |
| A11 | Dormant activation | User không có activity > 7 ngày, đột ngột hoạt động mạnh (> 20 event trong 1 giờ) |

---

## 4. Non-Functional Requirements

| NFR    | Mô tả                 | Ngưỡng                                        |
| ------ | --------------------- | --------------------------------------------- |
| NFR-01 | Thời gian xử lý query | < 10 giây trên 1M dòng                        |
| NFR-02 | Stability             | Không crash trên 1M dòng                      |
| NFR-03 | Memory                | Giải phóng sau mỗi chức năng                  |
| NFR-04 | Robustness            | Hoạt động đúng khi có dữ liệu invalid / trùng |

---

## 5. Technical Constraints

### Bắt buộc dùng

```cpp
struct       // Định nghĩa data model
T*           // Con trỏ
new / delete // Cấp phát / giải phóng thủ công
```

### Cấm dùng

```cpp
std::vector, std::map, std::unordered_map, std::set
// Mọi thư viện đồ thị
```

### Được phép

```cpp
std::string
std::ifstream, std::ofstream
std::cout, std::cin
```

---

## 6. Business Logic — Session Definition

```
Session bắt đầu: khi gặp event LOGIN
Session kết thúc: khi gặp LOGOUT, HOẶC không có event trong 30 phút
Nếu không có LOGIN rõ ràng: session bắt đầu từ event đầu tiên của ngày
```

---

## 7. CLI Interface

```
Commands:
  load <filepath>                           Load CSV file
  query user <user_id> <t_start> <t_end>   FR-03
  query resource <res_id> <t_start> <t_end> FR-04
  top resources <t_start> <t_end>           FR-05
  detect anomaly [--type=all|threshold|behavior|session|advanced]
  help                                      Liệt kê lệnh
  exit                                      Thoát, giải phóng bộ nhớ
```

---

## 8. DO / DON'T

| DO ✅                                  | DON'T ❌                         |
| -------------------------------------- | -------------------------------- |
| Giải phóng bộ nhớ sau mỗi thao tác     | Dùng vector/map                  |
| Kiểm tra nullptr trước khi dereference | Để memory leak                   |
| Skip dòng lỗi thay vì crash            | Crash khi gặp dữ liệu bất thường |
| Dùng hằng số có tên                    | Magic number                     |
| Comment giải thích thuật toán phức tạp | Code không có comment            |
| Build và test ở release mode           | Chỉ test ở debug mode            |

---

## 9. Scoring

| Yêu cầu                                                | Điểm                     |
| ------------------------------------------------------ | ------------------------ |
| FR-01 đến FR-05 (Midterm)                              | ~4 điểm                  |
| FR-06 đến FR-08 (Final threshold + behavior + session) | ~5 điểm                  |
| FR-09 (Advanced bonus)                                 | +2 điểm                  |
| Đề xuất anomaly mới thuyết phục                        | +0.2 đến +1 điểm mỗi cái |
| Vi phạm kỹ thuật (dùng vector/map, leak memory)        | Tối đa 2 điểm            |
| Giống bài khác ≥ 80%                                   | 1 điểm cả hai            |
