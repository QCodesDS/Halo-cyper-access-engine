# Phase 03 — Query Engine (Midterm)
# FR-03, FR-04, FR-05

**Thời lượng:** 3 ngày
**Prereq:** Phase 01 + 02 hoàn thành
**Mục tiêu:** Hoàn thiện 3 query midterm, nộp bài đúng hạn.

---

## Checklist

### Query Implementations
- [ ] `src/query/UserJourneyQuery.h/.cpp` — FR-03
- [ ] `src/query/ResourceJourneyQuery.h/.cpp` — FR-04
- [ ] `src/query/TopResourceQuery.h/.cpp` — FR-05
- [ ] `src/query/QueryEngine.h/.cpp` — dispatcher từ CLI

### CLI Integration
- [ ] Lệnh: `query user <uid> <t_start> <t_end>`
- [ ] Lệnh: `query resource <rid> <t_start> <t_end>`
- [ ] Lệnh: `top resources <t_start> <t_end>`
- [ ] Printer: format bảng đẹp, có header

---

## Expected Output

### query user U007 1713000000 1713999999
```
User Journey: U007 | 2024-04-13 to 2024-04-24
─────────────────────────────────────────────────────────────
Timestamp           Device   App      Resource   Event        Location
─────────────────────────────────────────────────────────────
2024-04-15 10:31:05 D018  →  APP003 → R025      DOWNLOAD     SG
2024-04-15 14:22:41 D018  →  APP003 → R011      ACCESS       SG
...
─────────────────────────────────────────────────────────────
Total: 23 events
```

### top resources 1713000000 1713999999
```
Top 10 Resources | 2024-04-13 to 2024-04-24
──────────────────────────────
Rank  Resource   Count
──────────────────────────────
1     R025       1842
2     R011       1703
3     R003       1590
...
```

---

## Manual Test Guide

### Test 1: User journey — user tồn tại
```
1. load data/sample.csv
2. query user U007 0 9999999999
3. Expect: danh sách event của U007, sorted by timestamp
4. Verify: grep "U007" sample.csv | wc -l == số dòng in ra
```

### Test 2: User journey — user không tồn tại
```
1. query user U999 0 9999999999
2. Expect: "[INFO] No events found for user U999."
```

### Test 3: Time range lọc đúng
```
1. query user U007 1713225863 1713225863  (exact 1 timestamp)
2. Expect: đúng 1 dòng
```

### Test 4: Top resources chính xác
```
1. Đếm thủ công bằng: grep "R025" sample.csv | wc -l
2. So sánh với output của top resources
3. Expect: khớp nhau
```

### Test 5: Memory sau query
```
Sau mỗi query, bộ nhớ intermediate phải được giải phóng.
Chạy 10 query liên tiếp → memory usage không tăng liên tục.
```

---

## Kỹ thuật quan trọng

### Count Map thủ công (cho Top Resources)
```cpp
// Không dùng std::map
// Tự cài đặt bằng dynamic array of {key, count} pairs
struct CountEntry {
    string resourceId;
    int count;
};

// Partial sort: chỉ cần top 10 → dùng selection sort 10 lần
// hoặc build heap thủ công nếu muốn tối ưu
```

### Filter trong time range
```cpp
// Dùng SortedIndex.binarySearchStart/End để tìm range
// Sau đó filter thêm theo user_id từ kết quả
// Không scan toàn bộ mảng nếu đã có sorted index
```

---

## Definition of Done

- [ ] 3 query đều trả kết quả đúng
- [ ] Edge cases: empty result, exact timestamp, invalid range
- [ ] Không memory leak sau mỗi query
- [ ] Output format rõ ràng, có header
- [ ] Build release → chạy được binary
- [ ] Update CHANGELOG + master-plan
- [ ] **MIDTERM SUBMITTED ✅**
