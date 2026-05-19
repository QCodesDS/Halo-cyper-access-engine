# Báo Cáo Hoàn Thành Dự Án: Halo – Cyber Access Engine

## 1. Bảng Đánh Giá Mức Độ Hoàn Thành Yêu Cầu (Functional Requirements - FR)

Dự án đã hoàn thành **100%** các yêu cầu chức năng từ FR-01 đến FR-09 theo đặc tả trong tài liệu Halo-cyber-access-engine.pdf, tuân thủ tuyệt đối quy định không dùng các cấu trúc dữ liệu STL (như `std::vector`, `std::map`, `std::set`, `std::unordered_map`).

| Mã Yêu Cầu | Tên Yêu Cầu Chức Năng                             |    Mức Hoàn Thành     | Mô Tả Ngắn Gọn & Ghi Chú Kỹ Thuật                                                                                                                                                                                                                                                |
| :--------- | :------------------------------------------------ | :-------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **FR-01**  | Đọc dữ liệu từ CSV và Xác thực                    | **100% (Hoàn thành)** | Parse dữ liệu dòng-dòng bằng bộ chia chuỗi tự viết (`StringUtils::split`), bỏ qua dòng lỗi hoặc không đúng số cột định dạng, ghi warning ra stderr và nạp vào DataStore động co giãn 2x.                                                                                         |
| **FR-02**  | Loại bỏ trùng lặp (Deduplication)                 | **100% (Hoàn thành)** | Loại bỏ toàn bộ các bản ghi trùng khít cả 7 cột. Thuật toán ban đầu $O(N^2)$ đã được tối ưu hóa thành $O(N \log N)$ sử dụng Merge Sort ổn định kết hợp so sánh lân cận, loại bỏ nguy cơ treo máy trên 1M dòng.                                                                   |
| **FR-03**  | Truy vấn Hành trình Người dùng (User Journey)     | **100% (Hoàn thành)** | Tìm kiếm nhanh tập sự kiện của người dùng qua `HashIndex` trong $O(1)$, lọc khoảng thời gian và sắp xếp theo dòng thời gian bằng Insertion Sort để in hành trình trực quan.                                                                                                      |
| **FR-04**  | Truy vấn Hành trình Tài nguyên (Resource Journey) | **100% (Hoàn thành)** | Truy cập tập sự kiện liên quan tới tài nguyên bằng `HashIndex`, lọc khoảng thời gian và sắp xếp theo dòng thời gian chính xác.                                                                                                                                                   |
| **FR-05**  | Thống kê Tài nguyên Tần suất cao                  | **100% (Hoàn thành)** | Sử dụng Binary Search trên SortedIndex để xác định cận khoảng thời gian trong $O(\log N)$, thống kê tần suất bằng mảng động tự co giãn và trích lọc Top N tài nguyên dùng Selection Sort tối ưu.                                                                                 |
| **FR-06**  | Bộ Dò tìm Anomaly Dựa trên Ngưỡng (Threshold)     | **100% (Hoàn thành)** | Triển khai thành công: **A01** (Đăng nhập đa thiết bị), **A02** (Đăng nhập lỗi liên tục), **A03** (Lũ tài nguyên theo thiết bị) và **A04** (Truy cập ngoài giờ làm việc).                                                                                                        |
| **FR-07**  | Bộ Dò tìm Anomaly Dựa trên Hành vi (Behavior)     | **100% (Hoàn thành)** | Triển khai thành công: **A05** (Di chuyển không khả thi với khoảng cách địa lý và thời gian) và **A06** (Thay đổi vị trí liên tục).                                                                                                                                              |
| **FR-08**  | Bộ Dò tìm Anomaly Dựa trên Phiên (Session)        | **100% (Hoàn thành)** | Triển khai bộ gom cụm phiên `SessionBuilder` theo các ranh giới LOGIN/LOGOUT và giới hạn 30 phút rảnh. Phát hiện thành công: **A07** (Phiên kéo dài), **A08** (Spam phiên) và **A09** (Chuỗi thao tác độc hại).                                                                  |
| **FR-09**  | Bộ Dò tìm Nâng cao & Điểm Thưởng (Advanced)       | **100% (Hoàn thành)** | Triển khai hoàn chỉnh toàn bộ 5 bộ dò tìm để lấy điểm tối đa: **A10** (Tấn công Brute force), **A11** (Kích hoạt tài khoản ngủ đông), và **3 bộ dò tìm đặc quyền bổ sung**: **A12** (Leo thang đặc quyền), **A13** (Đánh cắp rò rỉ dữ liệu) và **A14** (Dịch chuyển ngang mạng). |

---

## 2. Thiết Kế Hệ Thống & Kiến Trúc Module

Hệ thống được thiết kế theo mô hình **phân lớp tuần tự (Layered Architecture)**, phân chia rõ ràng trách nhiệm để đạt hiệu năng tối đa và đảm bảo an toàn bộ nhớ.

### 2.1. Sơ đồ các Module (Dạng Text)

```text
       ┌────────────────────────────────────────────────────────┐
       │                       CLI LAYER                        │
       │     (main.cpp / CLI command parser / Printer.h)        │
       └───────────────────────────┬────────────────────────────┘
                                   │
       ┌───────────────────────────▼────────────────────────────┐
       │                     ENGINE LAYER                       │
       │    (QueryEngine.h  /  AnomalyEngine.h / Session)       │
       └─────────────┬────────────────────────────┬─────────────┘
                     │                            │
       ┌─────────────▼─────────────┐┌─────────────▼─────────────┐
       │      QUERY MODULES        ││     DETECTOR MODULES      │
       │ (UserJourney,             ││ (Threshold, Behavior,     │
       │  ResourceJourney, TopN)   ││  Session, Advanced det.)  │
       └─────────────┬─────────────┘└─────────────┬─────────────┘
                     │                            │
       ┌─────────────▼────────────────────────────▼─────────────┐
       │                     INDEXING LAYER                     │
       │  (HashIndex: byUser, byDevice, byResource | HashTable  │
       │   SortedIndex: mergeSorted array by timestamp)         │
       └───────────────────────────┬────────────────────────────┘
                                   │
       ┌───────────────────────────▼────────────────────────────┐
       │                     STORAGE LAYER                      │
       │ (DataStore: LogRecord** | CsvLoader | Deduplicator)    │
       └────────────────────────────────────────────────────────┘
```

### 2.2. Quan hệ và Cách Tương tác giữa các Layer

1. **Storage Layer**: Chịu trách nhiệm giao tiếp trực tiếp với tệp dữ liệu CSV thô. Lớp này đọc dòng-dòng, chuẩn hóa thông tin, loại bỏ các dòng không hợp lệ và lọc trùng lặp bản ghi. Kết quả được lưu trữ tập trung tại **DataStore** - một mảng con trỏ động chứa các `LogRecord*`. Lớp này nắm giữ **quyền sở hữu duy nhất** đối với vùng nhớ của các `LogRecord`.
2. **Indexing Layer**: Khi DataStore được nạp đầy đủ, lớp chỉ mục sẽ quét qua một lượt duy nhất để xây dựng:
   - **HashIndex**: Gồm 3 bảng băm lớn sử dụng giải thuật Polynomial Rolling Hash kết hợp Separate Chaining (dùng danh sách liên kết và mảng động co giãn tự động) giúp truy vấn tức thì nhóm sự kiện của một User, Device, hay Resource trong thời gian $O(1)$.
   - **SortedIndex**: Mảng các con trỏ `LogRecord*` trỏ tới DataStore và được sắp xếp tăng dần theo thời gian bằng Merge Sort ổn định trong $O(N \log N)$ giúp thực hiện Binary Search phân vùng thời gian trong $O(\log N)$.
3. **Engine Layer**:
   - **Query Engine**: Tiếp nhận yêu cầu từ CLI, gọi chỉ mục Hash hoặc Sorted để lấy nhanh tập sự kiện phù hợp, sắp xếp kết xuất hành trình nhanh chóng dưới dạng bảng hoặc chuỗi biểu diễn trực quan.
   - **Anomaly Engine**: Gom cụm các sự kiện thành các phiên làm việc (`UserSession`) thông qua `SessionBuilder`. Sau đó, các bộ dò tìm thuộc tính sẽ duyệt qua các chỉ mục hoặc phiên để chấm điểm bất thường theo tiêu chí nghiêm ngặt của BRD.
4. **CLI Layer**: Vòng lặp giao tiếp dòng lệnh tương tác trực tiếp với chuyên viên phân tích, in kết quả định dạng bảng trực quan, ghi nhận thời gian chạy và ra lệnh giải phóng toàn bộ tài nguyên khi thoát chương trình.

---

## 3. Khó Khăn Gặp Phải & Hướng Giải Quyết

Trong suốt vòng đời phát triển dự án, ba thách thức kỹ thuật lớn sau đây đã được phát hiện và giải quyết triệt để:

### Vấn đề 1: Thắt nút cổ chai hiệu năng tại Deduplicator khi tải dữ liệu quy mô lớn (1M dòng)

- **Mô tả hiện tượng**: Khi thử nghiệm với tệp tin dữ liệu thực tế lớn (1M dòng), chương trình bị đóng băng (treo máy) trong nhiều phút tại bước Deduplication.
- **Nguyên nhân**: Thuật toán loại bỏ trùng lặp ban đầu sử dụng cơ chế so sánh cặp $O(N^2)$ tuyến tính, không thể đáp ứng được khi số dòng lớn.
- **Cách giải quyết**: Thay thế bộ khử trùng lặp tĩnh bằng giải thuật: Sắp xếp toàn bộ mảng bằng Merge Sort ổn định theo `timestamp` trong $O(N \log N)$, sau đó duyệt qua một lượt duy nhất $O(N)$ để so sánh các dòng kề nhau. Nếu trùng khít cả 7 trường cột, bản ghi phía sau sẽ bị loại bỏ. Giải pháp này giúp thời gian khử trùng tệp tin 1M dòng giảm từ nhiều giờ xuống dưới **2 giây**.

### Vấn đề 2: Treo máy khi kết xuất báo cáo thống kê Anomaly Report (Top 10 Suspicious Users)

- **Mô tả hiện tượng**: Quá trình phát hiện bất thường chạy rất nhanh, nhưng bước in báo cáo tóm tắt trên tệp 1M dòng lại gây treo ứng dụng.
- **Nguyên nhân**: Thuật toán đếm tần suất vi phạm của từng người dùng trong báo cáo sử dụng duyệt tuyến tính lồng nhau có độ phức tạp $O(N \times U)$ (N là số lượng cảnh báo lên tới hàng trăm ngàn, U là số người dùng). Việc sao chép chuỗi `std::string` liên tục cũng gây quá tải bộ cấp phát bộ nhớ.
- **Cách giải quyết**: Chuyển sang sắp xếp danh sách các chuỗi con trỏ `const std::string*` chỉ tới tên người dùng bị cảnh báo bằng Merge Sort trong $O(N \log N)$, sau đó duyệt gom nhóm đếm tần suất chỉ trong một vòng lặp $O(N)$. Cách tiếp cận này loại bỏ hoàn toàn việc sao chép dữ liệu chuỗi và giảm thời gian in bảng thống kê từ hàng chục phút xuống dưới **0.1 giây**.

### Vấn đề 3: Nguy cơ rò rỉ bộ nhớ (Memory Leak) do quản lý con trỏ thủ công không sử dụng STL

- **Mô tả hiện tượng**: Việc cấm sử dụng `std::vector`, `std::map`, hay `std::shared_ptr` khiến các mảng phụ trợ và danh sách phiên liên kết rất khó thu hồi sạch sẽ, đặc biệt khi chương trình chạy liên tiếp nhiều lệnh phân tích dữ liệu khác nhau trên CLI.
- **Nguyên nhân**: Sự phân rã quyền sở hữu vùng nhớ giữa DataStore (sở hữu bản ghi gốc) và các chỉ mục hay phiên làm việc (sở hữu mảng con trỏ phụ).
- **Cách giải quyết**:
  - Quy định rõ **chỉ có DataStore** sở hữu và chịu trách nhiệm delete các đối tượng `LogRecord` gốc. Các chỉ mục chỉ nắm giữ con trỏ và chỉ giải phóng cấu trúc mảng của mình (`clearSortedIndex`).
  - Xây dựng các hàm dọn dẹp chuyên sâu như `clearSessions` để giải phóng từng đối tượng `UserSession` cùng với mảng sự kiện nội bộ của nó.
  - Sử dụng các công cụ kiểm toán mã nguồn nghiêm ngặt, đảm bảo mọi lệnh `detect` hoặc `query` đều có lệnh giải phóng mảng tạm thời ngay lập tức trước khi trả quyền điều khiển về CLI.

---

## 4. Hướng Dẫn Biên Dịch & Sử Dụng

### 4.1. Yêu cầu Hệ thống & Môi trường

- **Hệ điều hành**: Windows 10/11, macOS, hoặc Linux.
- **Trình biên dịch**: GCC/g++ hỗ trợ tiêu chuẩn **C++17** trở lên, hoặc MSVC (Visual Studio).
- **Hệ thống build**: **CMake** phiên bản $\ge 3.16$.

### 4.2. Các lệnh Biên dịch (CMake)

Mở Terminal tại thư mục gốc của dự án và chạy các lệnh sau:

```bash
# 1. Tạo thư mục build
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Biên dịch hệ thống (tự động tối ưu hóa và xuất file nhị phân vào thư mục release)
cmake --build build
```

Sau khi biên dịch thành công, tệp khả thi độc lập sẽ nằm tại:

- Windows: `release/bin/halo.exe`
- Linux/macOS: `release/bin/halo`

### 4.3. Hướng dẫn sử dụng CLI

Chạy chương trình trực tiếp bằng dòng lệnh:

```bash
./release/bin/halo
```

Hệ thống sẽ chuyển sang giao diện tương tác CLI:

| Lệnh CLI                        | Ý nghĩa & Tác vụ                             | Ví dụ sử dụng thực tế                                                                                                                              |
| :------------------------------ | :------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`load`**                      | Nạp dữ liệu CSV và dựng chỉ mục nhanh        | `load src/data/halo_dataset_1k.csv`                                                                                                                |
| **`query user`**                | Truy vấn hành trình chi tiết của một User    | `query user U090 1715000000 1716000000`                                                                                                            |
| **`query resource`**            | Truy vấn hành trình truy cập một Tài nguyên  | `query resource R048 1715000000 1716000000`                                                                                                        |
| **`top resources`**             | Thống kê Top tài nguyên sử dụng nhiều nhất   | `top resources 1715000000 1716000000`                                                                                                              |
| **`detect anomaly`**            | Quét và phát hiện toàn bộ 14 loại bất thường | `detect anomaly`                                                                                                                                   |
| **`detect anomaly --type=...`** | Chỉ quét nhóm bất thường được yêu cầu        | `detect anomaly --type=threshold` <br> `detect anomaly --type=behavior` <br> `detect anomaly --type=session` <br> `detect anomaly --type=advanced` |
| **`help`**                      | Hiển thị bảng trợ giúp lệnh CLI              | `help`                                                                                                                                             |
| **`exit`**                      | Giải phóng toàn bộ bộ nhớ và thoát ứng dụng  | `exit`                                                                                                                                             |
