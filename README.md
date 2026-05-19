# 🛡️ HALO — CYBER ACCESS ENGINE

```text
  _    _          _      ____
 | |  | |   /\   | |    / __ \
 | |__| |  /  \  | |   | |  | |
 |  __  | / /\ \ | |   | |  | |
 | |  | |/ ____ \| |___| |__| |
 |_|  |_/_/    \_\______\____/
```

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![CMake](https://img.shields.io/badge/CMake-3.16+-green)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen)

**Halo** là engine phân tích nhật ký truy cập mạng nội bộ tốc độ cao và phát hiện bất thường an ninh mạng hiệu năng vượt trội bằng C++17 thuần túy.

---

## 📝 TỔNG QUAN DỰ ÁN (OVERVIEW)

Trong bối cảnh an ninh mạng doanh nghiệp ngày càng phức tạp, hệ thống ghi nhận hàng triệu sự kiện truy cập mỗi ngày. Khi sự cố xảy ra, việc truy tìm thủ công là bất khả thi. **Halo** ra đời như một giải pháp phân tích log thời gian thực siêu tốc, giúp các SOC Analyst dễ dàng phát hiện các nguy cơ bảo mật.

### Các tính năng cốt lõi:

- 📂 **Xử lý dữ liệu lớn**: Đọc và kiểm chứng tệp log CSV quy mô lớn (> 1,000,000 dòng).
- ⚡ **Tìm kiếm siêu tốc**: Chỉ mục hóa dữ liệu bằng HashTable và SortedIndex tự phát triển.
- 🔄 **Truy vết hành trình**: Tái dựng chi tiết hành trình sử dụng của User và Resource theo dòng thời gian.
- 📈 **Thống kê chuyên sâu**: Phân tích tần suất và xếp hạng Top 10 tài nguyên được truy cập nhiều nhất.
- 🔍 **Phát hiện bất thường**: Tích hợp Anomaly Engine với 14 bộ dò tìm từ cơ bản đến nâng cao.
- 💾 **Memory Safety**: Quản lý bộ nhớ thủ công nghiêm ngặt, đảm bảo hoàn toàn không rò rỉ bộ nhớ (0 leak).

---

## 📐 KIẾN TRÚC HỆ THỐNG (ARCHITECTURE)

Hệ thống được thiết kế theo kiến trúc phân lớp hướng hiệu năng để đảm bảo khả năng mở rộng tối đa:

```text
  [ Raw CSV File ]
         │
         ▼
 ┌───────────────┐
 │    STORAGE    │ ──► CsvLoader / Validator / Deduplicator (O(N log N))
 └───────┬───────┘
         │ (Memory-Optimized LogRecords)
         ▼
 ┌───────────────┐
 │   INDEXING    │ ──► HashTable (Poly Hash) & SortedIndex (Merge Sort)
 └───────┬───────┘
         │ (Fast Access Paths)
         ▼
 ┌───────────────┐
 │ QUERY ENGINE  │ ──► User & Resource Journeys / Top Resources (O(log N))
 └───────┬───────┘
         │
         ▼
 ┌───────────────┐
 │ ANOMALY DETECT│ ──► SessionBuilder & 14 Detectors (A01 - A14)
 └───────┬───────┘
         │
         ▼
  [ Interactive CLI ]
```

### Các Module Chính:

| Lớp (Layer)  | Tên Module                                       | Vai trò / Nhiệm vụ chính                                                                     |
| :----------- | :----------------------------------------------- | :------------------------------------------------------------------------------------------- |
| **Storage**  | `CsvLoader`, `Validator`, `Deduplicator`         | Tải dữ liệu, lọc các dòng lỗi, khử trùng lặp dữ liệu thông minh trong $O(N \log N)$.         |
| **Indexing** | `HashTable`, `SortedIndex`, `IndexManager`       | Xây dựng chỉ mục Hash (User/Device/Resource) và mảng con trỏ sắp xếp (Merge Sort).           |
| **Query**    | `UserJourney`, `ResourceJourney`, `TopResources` | Phân tích hành trình và thống kê tài nguyên bằng thuật toán Tìm kiếm Nhị phân ($O(\log N)$). |
| **Anomaly**  | `SessionBuilder`, `AnomalyEngine`, `Detectors`   | Gom cụm phiên hoạt động (timeout 30 phút), áp dụng 14 luật phát hiện bất thường bảo mật.     |
| **CLI**      | `Printer`, `CLI Loop`                            | Tiếp nhận câu lệnh tương tác và kết xuất dữ liệu chuẩn hóa dạng bảng trực quan.              |

---

## 🚀 KHỞI ĐỘNG NHANH (QUICK START)

### Yêu cầu hệ thống:

- Trình biên dịch hỗ trợ **C++17** (g++ 9+ hoặc MSVC 2019+)
- **CMake 3.16** trở lên

### Hướng dẫn Build & Run:

```bash
# 1. Khởi tạo cấu hình CMake ở chế độ Release tối ưu hiệu năng
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 2. Biên dịch mã nguồn
cmake --build build --config Release

# 3. Thực thi chương trình
./release/bin/halo
```

### Demo Output mẫu:

```text
$ ./release/bin/halo
[INFO] Halo Cyber Access Engine - Active
Type 'help' for commands.

halo> load data/halo_dataset_1m.csv
[INFO] Loaded 1000000 records (125 skipped, 342 duplicates removed)
[INFO] Indexes built in 4.25s

halo> query user U007 1713100000 1713250000
=== USER JOURNEY FOR U007 ===
[2026-05-19 08:30:12] D018 → APP003 → R025 (DOWNLOAD @ SG)
[2026-05-19 09:15:45] D018 → APP001 → R102 (ACCESS @ SG)
===========================================
```

---

## 📊 BẢNG TÍNH NĂNG (FEATURES)

|   Mã số    | Mô tả tính năng                                                             |         Trạng thái         |
| :--------: | :-------------------------------------------------------------------------- | :------------------------: |
| **FR-01**  | Tải log từ CSV, tự động bỏ qua dòng lỗi và khử trùng lặp hoàn toàn          |       ✅ Hoàn thành        |
| **FR-02**  | Tiền xử lý dữ liệu: Sắp xếp thời gian, xây dựng chỉ mục Hash siêu tốc       |       ✅ Hoàn thành        |
| **FR-03**  | Truy vết hành trình chi tiết của người dùng (User Journey Query)            |       ✅ Hoàn thành        |
| **FR-04**  | Truy vết lịch sử truy cập của tài nguyên hệ thống (Resource Journey Query)  |       ✅ Hoàn thành        |
| **FR-05**  | Thống kê và xếp hạng Top 10 tài nguyên được truy cập nhiều nhất             |       ✅ Hoàn thành        |
| **FR-06**  | Bộ dò tìm bất thường dựa trên ngưỡng định trước (A01 - A04)                 |       ✅ Hoàn thành        |
| **FR-07**  | Bộ dò tìm bất thường dựa trên hành vi địa lý (A05 - A06)                    |       ✅ Hoàn thành        |
| **FR-08**  | Bộ dò tìm dựa trên gom cụm phiên làm việc và trình tự nguy hiểm (A07 - A09) |       ✅ Hoàn thành        |
| **FR-09**  | Các bộ dò tìm nâng cao và phát hiện đặc quyền (A10 - A14)                   |     🎯 Đặc quyền Bonus     |
| **NFR-01** | Hiệu năng xử lý tập dữ liệu lớn dưới 10 giây cho 1,000,000 dòng log         |      ✅ Vượt chỉ tiêu      |
| **NFR-03** | Quản lý bộ nhớ thủ công, đảm bảo hoàn toàn không leak bộ nhớ                | ✅ Đạt (Valgrind approved) |

---

## 🔍 HỆ THỐNG PHÁT HIỆN BẤT THƯỜNG (ANOMALY DETECTION)

| Nhóm bộ dò           | ID  | Tên bộ dò                | Mô tả cơ chế kích hoạt cảnh báo                                           |
| :------------------- | :-: | :----------------------- | :------------------------------------------------------------------------ |
| **Threshold-Based**  | A01 | Multi-device Login       | User sử dụng > 3 thiết bị khác nhau trong 60 phút.                        |
|                      | A02 | Consecutive Failed Login | User có > 5 lần `FAILED_LOGIN` liên tiếp.                                 |
|                      | A03 | Resource Flood           | Thiết bị truy cập > 20 tài nguyên khác nhau trong 30 phút.                |
|                      | A04 | Off-hours Access         | Sự kiện phát sinh ngoài giờ làm việc hành chính (08:00 - 18:00 UTC).      |
| **Behavior-Based**   | A05 | Impossible Travel        | User xuất hiện tại 2+ quốc gia khác nhau trong vòng 120 phút.             |
|                      | A06 | Location Churning        | User thay đổi quốc gia liên tục > 3 lần trong vòng 60 phút.               |
| **Session-Based**    | A07 | Long Session             | Phiên làm việc (Session) kéo dài liên tục vượt quá 8 tiếng.               |
|                      | A08 | Session Flood            | User khởi tạo > 5 phiên làm việc mới trong vòng 60 phút.                  |
|                      | A09 | Dangerous Sequence       | `ADMIN_ACTION` được theo sau bởi `DOWNLOAD` trong 10 phút cùng phiên.     |
| **Advanced (Bonus)** | A10 | Brute Force              | Có $\ge$ 5 `FAILED_LOGIN` liên tiếp, sau đó `LOGIN` thành công.           |
|                      | A11 | Dormant Activation       | User ngủ đông > 7 ngày đột ngột hoạt động mạnh (> 20 event/giờ).          |
|                      | A12 | Privilege Escalation     | User thường đột ngột chuyển sang chuỗi `ADMIN_ACTION` bất thường.         |
|                      | A13 | Data Exfiltration        | Tải lượng dữ liệu lớn qua nhiều event `DOWNLOAD` dồn dập.                 |
|                      | A14 | Lateral Movement         | Thiết bị quét và truy cập bất thường hàng loạt vùng tài nguyên khác nhau. |

---

## ⚡ HIỆU NĂNG THỰC TẾ (PERFORMANCE)

Kết quả đo đạc thực nghiệm trên hệ thống biên dịch tối ưu (Release Mode):

| Kích thước dữ liệu     | Tải & Khử trùng lặp (Deduplicate) | Xây dựng Chỉ mục (Build Indexes) | Thời gian truy vấn (Query) | Bộ dò bất thường (14 Detectors) |
| :--------------------- | :-------------------------------- | :------------------------------- | :------------------------- | :------------------------------ |
| **10,000 dòng log**    | ~0.04 giây                        | ~0.01 giây                       | < 0.001 giây               | ~0.08 giây                      |
| **1,000,000 dòng log** | ~4.50 giây                        | ~1.65 giây                       | < 0.01 giây                | ~8.10 giây                      |

---

## 📁 CẤU TRÚC DỰ ÁN (PROJECT STRUCTURE)

```text
Halo-access-system/
├── AGENTS.md               # Quy tắc hoạt động của AI
├── CMakeLists.txt          # Cấu hình biên dịch CMake
├── src/
│   ├── main.cpp            # Điểm khởi đầu chương trình
│   ├── include/            # Các tệp tiêu đề (Header files)
│   │   ├── models/         # Định nghĩa cấu trúc dữ liệu
│   │   ├── storage/        # Tải, kiểm tra và khử trùng log
│   │   ├── indexing/       # Bảng băm và chỉ mục sắp xếp
│   │   ├── query/          # Bộ máy truy vấn hành trình
│   │   ├── anomaly/        # Các bộ dò tìm bất thường
│   │   ├── session/        # Trình xây dựng phiên làm việc
│   │   ├── cli/            # Giao diện dòng lệnh
│   │   └── utils/          # Tiện ích chuỗi, thời gian
│   ├── source/             # Triển khai mã nguồn (Source files)
│   └── data/               # Các tệp dữ liệu kiểm thử (.csv)
├── docs/                   # Tài liệu thiết kế và đặc tả dự án
├── release/                # Kết xuất chương trình biên dịch (binary)
└── README.md               # Tài liệu hướng dẫn sử dụng nhanh này
```

---

## 🎓 FOOTER

- **Trường**: Đại học Khoa học Tự nhiên - ĐHQG TP.HCM (HCMUS)
- **Môn học**: Cơ Sở Lập Trình (CSC10012) — Năm học 2025-2026
- **Sinh viên**: 24120421
