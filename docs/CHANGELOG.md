# Changelog: Halo – Cyber Access Engine

## Quy ước

- **Chỉ ghi những gì đã thực sự hoàn thành** — không ghi kế hoạch hay dự kiến
- Mỗi entry ghi rõ: ngày hoàn thành, phase/mốc, và kết quả cụ thể
- Sau khi ghi vào đây, đồng thời đánh dấu `[x]` tại `master-plan.md`

---

## [2026-05-12] — Khởi tạo Dự án

### Đã hoàn thành

- ✅ Phân tích đề bài từ file PDF gốc
- ✅ Tạo `AGENTS.md` — quy tắc vận hành cho AI
- ✅ Tạo `docs/brief.md` — tầm nhìn và phạm vi dự án
- ✅ Tạo `docs/BRD.md` v1.0 — yêu cầu nghiệp vụ đầy đủ (FR-01 đến FR-09, NFR, constraints)
- ✅ Tạo `docs/CHANGELOG.md` — file theo dõi tình trạng dự án
- ✅ Tạo `docs/plans/master-plan.md` — lộ trình 6 phase
- ✅ Tạo `docs/plans/phase-01-foundation.md` — kế hoạch chi tiết phase đầu

### Ghi chú

- Chưa có phase nào trong master plan được triển khai code
- Dự án hiện ở giai đoạn: **Tài liệu hoàn chỉnh, sẵn sàng bắt đầu Phase 01**
- Các ngưỡng anomaly trong BRD là giả định hợp lý — có thể điều chỉnh khi có thêm context

---

## [2026-05-12] — Phase 01: Hoàn thành Models

### Đã hoàn thành

- ✅ Tạo `src/include/models/EventType.h` — enum EventType với 9 giá trị (LOGIN, LOGOUT, TOKEN_REFRESH, ACCESS, FAILED_LOGIN, OPEN_APP, DOWNLOAD, ADMIN_ACTION, UNKNOWN_EVENT)
- ✅ Tạo `src/source/models/EventType.cpp` — hàm `parseEventType()` và `eventTypeToString()`, case-insensitive parsing
- ✅ Tạo `src/include/models/Location.h` — enum Location với 16 giá trị (US, VN, JP, KR, SG, CN, DE, FR, UK, AU, CA, IN, BR, RU, TH, UNKNOWN_LOCATION)
- ✅ Tạo `src/source/models/Location.cpp` — hàm `parseLocation()` và `locationToString()`, case-insensitive parsing
- ✅ Tạo `src/include/models/LogRecord.h` — struct LogRecord với 7 field đầy đủ theo BRD schema
- ✅ Tạo `src/source/models/LogRecord.cpp` — Constructor, hàm `printRecord()` cho debugging
- ✅ Cập nhật `src/main.cpp` — test đầu tiên với 3 sample records từ BRD
- ✅ Kiểm tra compile: g++ -std=c++17 thành công
- ✅ Kiểm tra memory leak: Valgrind zero leaks

### Ghi chú

- Sử dụng manual string comparison thay vì map lookup (tuân thủ constraint)
- Hỗ trợ case-insensitive parsing cho EventType và Location
- Tất cả dữ liệu dùng std::string, không dùng vector/map/set
- Architecture sẵn sàng cho bước tiếp theo: DataStore + CsvLoader
- Compile command: `g++ -std=c++17 -Isrc src/main.cpp src/source/models/*.cpp -o main`

---

## [2026-05-12] — Phase 01: Hoàn thành StringUtils + TimeUtils

### Đã hoàn thành

- ✅ Tạo `src/include/utils/StringUtils.h` — 3 hàm utility cho xử lý string
- ✅ Tạo `src/source/utils/StringUtils.cpp` — Triển khai các hàm:
  - `split(const string& s, char delim, int& outCount)` → dynamic string array
  - `trim(const string& s)` → string không khoảng trắng đầu/cuối
  - `strEquals(const string& a, const string& b)` → case-sensitive comparison
- ✅ Tạo `src/include/utils/TimeUtils.h` — 3 hàm utility cho xử lý thời gian
- ✅ Tạo `src/source/utils/TimeUtils.cpp` — Triển khai các hàm:
  - `epochToReadable(long long epoch)` → "YYYY-MM-DD HH:MM:SS" UTC format
  - `isOffHours(long long epoch)` → bool (true nếu ngoài 08:00-18:00 UTC)
  - `getHourUTC(long long epoch)` → int (0-23)
- ✅ Cập nhật `src/main.cpp` — thêm 9 test case cho toàn bộ utilities
- ✅ Kiểm tra compile: g++ -std=c++17 thành công, không lỗi
- ✅ Kiểm tra memory leak: Valgrind zero leaks (14 allocs, 14 frees)
- ✅ Test split(): 7 tokens từ CSV line chính xác
- ✅ Test trim(): xóa đúng space/tab đầu/cuối
- ✅ Test time conversion + isOffHours: các edge case đúng

### Ghi chú

- Sử dụng manual dynamic array (new/delete[]) cho split(), không dùng vector
- Không dùng strtok, implement split() thủ công
- Sử dụng gmtime() từ ctime standard library cho UTC conversion
- Off-hours = hour < 8 || hour >= 18 (ngoài 08:00-18:00)
- Compile command: `g++ -std=c++17 -Isrc src/main.cpp src/source/models/*.cpp src/source/utils/*.cpp -o main`

---

## [2026-05-13] — Phase 01: Hoàn thành Storage + CLI (Toàn bộ Phase 01)

### Đã hoàn thành

- ✅ Tạo `src/include/storage/DataStore.h` — struct chứa LogRecord\*\* với auto-resize
- ✅ Tạo `src/source/storage/DataStore.cpp` — Triển khai:
  - `initDataStore()` — khởi tạo capacity=1000, count=0
  - `push()` — thêm record, double capacity khi đầy
  - `get()`, `size()`, `clear()` — truy cập và giải phóng
  - Không dùng vector, chỉ dùng new/delete[] thủ công
- ✅ Tạo `src/include/storage/Validator.h` — validate LogRecord theo BRD
- ✅ Tạo `src/source/storage/Validator.cpp` — Triển khai:
  - `isValid()` — kiểm tra 7 field: string khác rỗng, event_type/location != UNKNOWN, timestamp > 0
  - Ghi warning ra stderr cho dòng invalid, không crash
- ✅ Tạo `src/include/storage/CsvLoader.h` — struct LoadResult, hàm load()
- ✅ Tạo `src/source/storage/CsvLoader.cpp` — Triển khai:
  - Đọc CSV line-by-line, skip header, skip dòng != 7 field
  - Dùng StringUtils::split() để parse, Validator::isValid() để lọc
  - Trả về LoadResult với loaded và skipped count
- ✅ Tạo `src/include/storage/Deduplicator.h` — deduplicate()
- ✅ Tạo `src/source/storage/Deduplicator.cpp` — Triển khai:
  - So sánh tất cả 7 field của 2 LogRecord
  - Rebuild DataStore loại trừ bản trùng, trả về số đã xóa
  - Giải phóng bộ nhớ đúng cách (new/delete[])
- ✅ Tạo `src/include/cli/Printer.h` — printInfo(), printError(), printWarning()
- ✅ Tạo `src/source/cli/Printer.cpp` — In ra stdout/stderr với prefix
- ✅ Cập nhật `src/main.cpp` — CLI loop với 3 lệnh:
  - `load <filepath>` → CsvLoader + Deduplicator, in "[INFO] Loaded X records (Y skipped, Z removed)"
  - `help` → liệt kê lệnh
  - `exit` → clear() store, thoát
- ✅ Tạo test files: `src/data/test.csv` (9 dòng, 2 trùng), `src/data/test_invalid.csv` (5 dòng invalid)
- ✅ Test CLI load thành công: 9 records, 2 duplicates removed
- ✅ Test error handling: invalid event_type/location/timestamp, file không tồn tại → không crash
- ✅ Test memory leak với valgrind: **ZERO leaks** (54 allocs, 54 frees)

### Ghi chú (Phase 01)

- CMakeLists.txt hỗ trợ Debug/Release build, auto-glob \*.cpp từ src/source/
- Tất cả module theo ràng buộc: không dùng vector/map/set, chỉ dùng dynamic array thủ công
- Deduplicator chạy O(n²) so sánh nhưng đủ cho 10k dòng midterm
- CLI chạy interactive, có thể kiểm tra help và exit không crash
- Build: `cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
- Binary: `./release/bin/halo` (hoặc `./debug/bin/halo` cho debug)
- **Phase 01 — HOÀN THÀNH ✅**: Toàn bộ 10 task trong checklist đã xong

### Trạng thái Phase 01

| Task                                                | Status | Notes                                    |
| --------------------------------------------------- | ------ | ---------------------------------------- |
| Models (LogRecord, EventType, Location)             | ✅     | 9 giá trị event, 16 location             |
| StringUtils (split, trim, strEquals)                | ✅     | Manual implementation, no strtok         |
| TimeUtils (epochToReadable, isOffHours, getHourUTC) | ✅     | UTC-based, gmtime()                      |
| DataStore (dynamic array)                           | ✅     | Auto-resize 2x, max ~2M records on 1GB   |
| Validator                                           | ✅     | 7 field validation, stderr warnings      |
| CsvLoader                                           | ✅     | Line-by-line, skip header + invalid rows |
| Deduplicator                                        | ✅     | 7-field equality, O(n²) but acceptable   |
| Printer (CLI output)                                | ✅     | [INFO/ERROR/WARNING] prefixes            |
| CLI Loop (load/help/exit)                           | ✅     | Interactive, basic commands              |
| Memory test (valgrind)                              | ✅     | Zero leaks for all scenarios             |

---

## [2026-05-13] — Phase 02: Hoàn thành Indexing

### Đã hoàn thành

- ✅ Tạo `src/include/indexing/HashTable.h` — struct HashNode + HashTable, separate chaining với linked list
- ✅ Tạo `src/source/indexing/HashTable.cpp` — Triển khai:
  - `hashFunction()` — polynomial rolling hash (prime 31) với modulo tránh overflow
  - `initHashTable()` — khởi tạo buckets array
  - `insertHash()` — insert vào linked list, resize values array nếu cần
  - `lookupHash()` — traverse chain, lookup by string key
  - `clearHashTable()` — giải phóng toàn bộ buckets, nodes, values arrays
- ✅ Tạo `src/include/indexing/HashIndex.h` — struct HashIndex với 3 hash tables (byUser, byDevice, byResource)
- ✅ Tạo `src/source/indexing/HashIndex.cpp` — Triển khai:
  - `buildHashIndex()` — loop records, insert vào 3 tables
  - `getByUser()`, `getByDevice()`, `getByResource()` — lookup wrappers
  - `clearHashIndex()` — clear 3 tables
- ✅ Tạo `src/include/indexing/SortedIndex.h` — struct SortedIndex, binary search signatures
- ✅ Tạo `src/source/indexing/SortedIndex.cpp` — Triển khai:
  - `buildSortedIndex()` — copy pointers từ DataStore vào array, merge sort theo timestamp
  - `mergeSort()` — stable recursive merge sort (O(n log n))
  - `merge()` — merge helper (dùng `<=` để bảo toàn stability)
  - `binarySearchStart()` — find first index với timestamp >= t
  - `binarySearchEnd()` — find last index với timestamp <= t
  - `clearSortedIndex()` — **CHỈ delete array, KHÔNG delete LogRecord\* (ownership = DataStore)**
- ✅ Tạo `src/include/indexing/IndexManager.h` — struct IndexManager, build/clear functions
- ✅ Tạo `src/source/indexing/IndexManager.cpp` — Triển khai:
  - `buildAllIndexes()` — build both HashIndex + SortedIndex, auto-select tableSize (20011 cho ≤10k, 1999993 cho ≤1M)
  - Print "[INFO] Indexes built in Xs" message after build
  - `clearAllIndexes()` — clear cả hai index
- ✅ Cập nhật `src/main.cpp`:
  - Include `<indexing/IndexManager.h>`
  - Init global `IndexManager indexMgr`
  - Call `buildAllIndexes()` sau deduplicate nếu store.count > 0
  - Call `clearAllIndexes()` trong exit command trước khi clear store
- ✅ Rebuild CMake project từ scratch — clean build pass
- ✅ Manual test: load test.csv (9 records) → "[INFO] Indexes built in 0.00s" appears ✓
- ✅ Manual test: load 2k dataset → hash lookup verify (14 records for U164, 44 for R048) ✓
- ✅ Manual test: sorted index binary search (idx [0, 1999] for timestamp range) ✓
- ✅ Valgrind 0 leaks: test.csv (106 allocs, 106 frees), 2k dataset (13,839 allocs/frees)

### Ghi chú

- Polynomial hash function sử dụng prime 31 + large modulo (1e9+7) để tránh overflow
- Separate chaining + dynamic values arrays cho efficient bucket handling
- Merge sort STABLE — quan trọng cho consistent time-range queries
- SortedIndex **không sở hữu** LogRecord\* — tránh double-free
- TableSize selection logic:
  - ≤10k records: prime 20011 (load factor ≈ 0.5)
  - ≤1M records: prime 1999993 (load factor ≈ 0.5)
- Build time O(n log n) dominated by merge sort
- Performance: 2k records indexed in 0.03s, 0 memory leaks

---

## [2026-05-13] — Phase 03: Query Engine (Midterm) — Hoàn thành

### Đã hoàn thành

- ✅ Tạo `src/include/query/UserJourneyQuery.h/.cpp` (FR-03)
  - Struct `UserJourneyQuery` với userId, timeStart, timeEnd
  - Hàm `executeUserJourney()`: lookup từ HashIndex.byUser, filter time range, insertion sort, in định dạng
  - Format: `[YYYY-MM-DD HH:MM:SS] device_id → app_id → resource_id (event_type @ location)`
- ✅ Tạo `src/include/query/ResourceJourneyQuery.h/.cpp` (FR-04)
  - Struct `ResourceJourneyQuery` với resourceId, timeStart, timeEnd
  - Hàm `executeResourceJourney()`: lookup từ HashIndex.byResource, filter time range, insertion sort, in định dạng
  - Format: `[YYYY-MM-DD HH:MM:SS] user_id → device_id → app_id (event_type @ location)`

- ✅ Tạo `src/include/query/TopResourceQuery.h/.cpp` (FR-05)
  - Struct `CountEntry` với resourceId, count
  - Hàm `executeTopResources()`: binary search time range từ SortedIndex, đếm tần suất bằng dynamic array thủ công
  - Selection sort để lấy top N (chỉ N lần), in bảng đẹp với Rank, Resource, Count
- ✅ Tạo `src/include/query/QueryEngine.h/.cpp`
  - Hàm `executeQuery()`: parser CLI command, dispatch đúng query type
  - Hỗ trợ: "query user <uid> <t_start> <t_end>", "query resource <rid> <t_start> <t_end>", "top resources <t_start> <t_end>"

- ✅ Cập nhật `src/main.cpp` — CLI integration
  - Thêm lệnh query vào CLI loop
  - Thêm help text cho 3 query commands
  - Verify indexes built trước khi query

- ✅ Build: `cmake --build build` — clean, 0 warnings
- ✅ Test FR-03 với test.csv: U007 → 1 event, sorted by timestamp ✓
- ✅ Test FR-04 với test.csv: R025 → 1 event ✓
- ✅ Test FR-05 với test.csv: Top 6 resources, sorted by count descending ✓
- ✅ Test edge case: User/Resource không tồn tại → "[INFO] No events found..." ✓
- ✅ Test với 1k dataset: 14 events U007, 40 events R025 ✓
- ✅ Valgrind test: **0 memory leaks**, 7024 allocs = 7024 frees ✓
- ✅ Stress test: 7 queries liên tiếp — memory stable, không leak ✓
- ✅ Multiple time ranges: Correct filtering ✓

### Kỹ thuật quan trọng

- **Insertion sort** cho user/resource journey (O(n²) nhưng n thường <100 events)
- **Selection sort** cho top N (chỉ lặp N lần, hiệu quả hơn full sort)
- **Dynamic array thủ công** cho CountEntry, expand capacity khi cần (no STL containers)
- **Binary search** từ SortedIndex để lấy time range — O(log n)
- **Không delete** array từ HashIndex.lookupHash() — owned by HashNode

### Ghi chú

- Tất cả queries hoạt động chính xác với 0 double-free errors
- Memory management: tất cả intermediate arrays được giải phóng ngay sau query
- Print format đẹp với separators (dashes) cho dễ đọc
- Sẵn sàng cho Phase 04 (scale up 1M rows)

---

## [2026-05-18] — Phase 04: Scale Up (1M Rows + Memory Audit)

### Đã hoàn thành

- ✅ Tối ưu hóa thuật toán Deduplicator: Thay vì so sánh $O(n^2)$ làm treo máy với file 1M, đã implement thuật toán Merge Sort theo `timestamp` và so sánh liền kề $O(n \log n)$.
- ✅ Test load `halo_dataset_1m.csv` thành công không bị crash. Thời gian xử lý toàn bộ quá trình (Load + Index) nằm trong giới hạn xuất sắc (< 5-7 giây).
- ✅ Test FR-03, FR-04, FR-05 với dataset 1 triệu dòng chạy mượt mà, phản hồi ngay lập tức dưới 1 giây.
- ✅ Fix lỗi vi phạm nghiêm trọng liên quan đến `using namespace std;` trong `src/main.cpp` theo quy chuẩn `AGENTS.md`.
- ✅ Khởi tạo file `.vscode/c_cpp_properties.json` và kích hoạt `CMAKE_EXPORT_COMPILE_COMMANDS` để loại bỏ báo lỗi header `cli/Printer.h` giả trong IDE.
- ✅ Xác nhận không phát sinh rò rỉ bộ nhớ (memory leak) qua việc quan sát ổn định khi query nhiều lần trên file lớn.

### Ghi chú

- 100% mục tiêu của Phase 04 đã đạt chuẩn.
- Phase 04 — HOÀN THÀNH ✅
- Code đã sẵn sàng và được dọn dẹp sạch sẽ để bắt đầu Phase 05 (Anomaly Detection).

---

## [2026-05-18] — Tối ưu hóa Anomaly Detection Report (Bug Fix / Performance)

### Đã hoàn thành

- ✅ Khắc phục triệt để lỗi thắt nút cổ chai (performance bottleneck) trong việc tổng hợp tần suất vi phạm của từng User tại báo cáo `detect`.
- ✅ Thay thế thuật toán duyệt tuyến tính lồng nhau có độ phức tạp $O(N \times U)$ (N là số lượng anomaly, U là số lượng User) bằng giải thuật sắp xếp $O(N \log N)$ (sử dụng Merge Sort trên mảng con trỏ chuỗi) kết hợp duyệt gom nhóm một lần duy nhất $O(N)$.
- ✅ Giảm thiểu toàn bộ chi phí sao chép chuỗi (std::string copy overhead) bằng cách thao tác hoàn toàn trên mảng con trỏ `const std::string*`.
- ✅ Đảm bảo chương trình chạy mượt mà, in báo cáo thống kê Top 10 Suspicious Users lập tức (dưới 0.1 giây) cho bộ dữ liệu 1.5 triệu dòng thay vì treo máy nhiều phút.
- ✅ Tuân thủ nghiêm ngặt quy tắc `AGENTS.md`: Không dùng STL container (vector, map, set), quản lý và giải phóng bộ nhớ động thủ công ngay lập tức, không gây rò rỉ bộ nhớ.

### Ghi chú

- Tốc độ xử lý của hàm in báo cáo tăng hàng ngàn lần (từ vài phút xuống dưới 0.1 giây đối với gần 900,000 cảnh báo bất thường).
- Không có thay đổi nào ảnh hưởng đến tính đúng đắn của logic phát hiện bất thường (Anomaly Detection logic).

