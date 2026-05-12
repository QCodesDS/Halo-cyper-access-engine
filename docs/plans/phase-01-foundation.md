# Phase 01 — Foundation

# Models + Storage + CLI Skeleton

**Thời lượng:** 3 ngày
**Mục tiêu:** Hệ thống có thể load file CSV, validate, lưu vào RAM và hiển thị CLI cơ bản.

---

## Checklist

### Models

- [ ] `src/include/models/LogRecord.h` — struct đầy đủ 7 field
- [ ] `src/source/models/LogRecord.cpp` — Triển khai (implementation) của struct LogRecord
- [ ] `src/include/models/EventType.h` — enum EventType (8 giá trị)
- [ ] `src/source/models/EventType.cpp` — Triển khai của enum EventType (nếu cần)
- [ ] `src/include/models/Location.h` — enum Location (15 giá trị)
- [ ] `src/source/models/Location.cpp` — Triển khai của enum Location (nếu cần)

### Storage

- [ ] `src/include/storage/DataStore.h` — dynamic array LogRecord\*
- [ ] `src/source/storage/DataStore.cpp` — Triển khai của dynamic array LogRecord\*
  - `push(LogRecord* r)` — thêm 1 record
  - `get(int i)` — truy cập theo index
  - `size()` — số lượng record
  - `clear()` — giải phóng toàn bộ
  - Resize tự động: double capacity khi đầy
- [ ] `src/include/storage/CsvLoader.h` — đọc file CSV dòng per dòng
- [ ] `src/source/storage/CsvLoader.cpp` — Triển khai của CsvLoader
  - `load(const string& path, DataStore& store)` — trả về số dòng đọc được
  - Xử lý: skip header, split by comma, trim whitespace
- [ ] `src/include/storage/Validator.h` — validate từng LogRecord
- [ ] `src/source/storage/Validator.cpp` — Triển khai của Validator
  - `isValid(const LogRecord& r)` — trả về bool
  - Log warning ra stderr cho dòng invalid (không crash)
- [ ] `src/include/storage/Deduplicator.h` — dedup sau khi load
- [ ] `src/source/storage/Deduplicator.cpp` — Triển khai của Deduplicator
  - `deduplicate(DataStore& store)` — loại bỏ dòng trùng, trả về số đã xóa

### Utils

- [ ] `src/include/utils/StringUtils.h` — split, trim, toLower
- [ ] `src/source/utils/StringUtils.cpp` — Triển khai của StringUtils
  - `split(const string& s, char delim)` → array of string
  - `trim(const string& s)` → string
  - `toLower(const string& s)` → string
- [ ] `src/include/utils/TimeUtils.h` — epochToReadable, isOffHours
- [ ] `src/source/utils/TimeUtils.cpp` — Triển khai của TimeUtils
  - `epochToReadable(long long epoch)` → string "YYYY-MM-DD HH:MM:SS"
  - `isOffHours(long long epoch)` → bool (ngoài 08:00–18:00 UTC)

### CLI

- [ ] `src/include/cli/CliHandler.h` — parse lệnh từ stdin
- [ ] `src/source/cli/CliHandler.cpp` — Triển khai của CliHandler
  - `parse(const string& line, DataStore& store)` — parse lệnh từ stdin
- [ ] `src/include/cli/Printer.h` — in kết quả ra stdout
- [ ] `src/source/cli/Printer.cpp` — Triển khai của Printer
  - `print(const DataStore& store)` — in kết quả ra stdout
- [ ] `src/main.cpp` — vòng lặp CLI chính

### Lệnh CLI Phase 01

```
load <filepath>    — Load CSV, in: "Loaded X records (Y skipped, Z duplicates removed)"
exit               — Giải phóng bộ nhớ, thoát chương trình
help               — Liệt kê lệnh
```

---

## Expected Output

```
> load data/sample.csv
[INFO] Loaded 9847 records (153 skipped, 12 duplicates removed)
[INFO] Ready.

> exit
[INFO] Memory freed. Goodbye.
```

---

## Manual Test Guide

### Test 1: Load file hợp lệ

```
1. Chuẩn bị file data/sample.csv có 100 dòng valid
2. Chạy: load data/sample.csv
3. Expect: "Loaded 100 records (0 skipped, 0 duplicates removed)"
```

### Test 2: File có dòng invalid

```
1. Thêm vào CSV: dòng thiếu cột, timestamp âm, event_type lạ
2. Expect: số skipped tăng lên đúng, chương trình không crash
```

### Test 3: File có dòng trùng

```
1. Copy y nguyên 5 dòng vào cuối file
2. Expect: "5 duplicates removed"
```

### Test 4: File không tồn tại

```
1. Chạy: load data/khongco.csv
2. Expect: "[ERROR] File not found." — chương trình tiếp tục chạy
```

### Test 5: Memory leak check

```
1. Load file → exit
2. Nếu dùng valgrind: valgrind --leak-check=full ./halo
3. Expect: "All heap blocks were freed"
```

---

## Kỹ thuật quan trọng

### Dynamic Array (DataStore)

```cpp
// Không dùng vector — phải tự implement
struct DataStore {
    LogRecord** records;  // mảng con trỏ
    int capacity;
    int count;
};

// Khi đầy: realloc gấp đôi
void resize(DataStore* store) {
    int newCap = store->capacity * 2;
    LogRecord** newArr = new LogRecord*[newCap];
    for (int i = 0; i < store->count; i++)
        newArr[i] = store->records[i];
    delete[] store->records;
    store->records = newArr;
    store->capacity = newCap;
}
```

### String Split thủ công

```cpp
// Không dùng stringstream hay strtok_r phức tạp
// Split bằng vòng lặp tìm delimiter
```

### Dedup Strategy

```
Option A (đơn giản): Sort → so sánh liên tiếp — O(n log n)
Option B: Hash toàn bộ record → O(n) nhưng phức tạp hơn
→ Dùng Option A cho Phase 01, tối ưu sau nếu cần
```

---

## Dependency

```
Phase 01 không phụ thuộc phase nào trước
Phase 02, 03, 04, 05 đều phụ thuộc Phase 01
```

---

## Definition of Done

- [ ] Tất cả test cases trên pass
- [ ] Không có warning compiler (g++ -Wall -Wextra)
- [ ] Không memory leak khi load và exit
- [ ] Code có comment giải thích các đoạn non-trivial
- [ ] Update CHANGELOG.md và master-plan.md
