# Master Plan — Halo Cyber Access Engine

## Trạng thái tổng quan

| Phase | Tên                                          | Deadline   | Trạng thái      |
| ----- | -------------------------------------------- | ---------- | --------------- |
| 01    | Foundation — Models + Storage + CLI skeleton | Ngày 1–3   | ✅ Hoàn thành   |
| 02    | Indexing — Hash Table + Sorted Index         | Ngày 4–5   | ✅ Hoàn thành   |
| 03    | Query Engine (Midterm)                       | Ngày 6–8   | ✅ Hoàn thành   |
| 04    | Scale Up — 1M rows + Memory audit            | Ngày 9–10  | ✅ Hoàn thành   |
| 05    | Anomaly Detection (Final)                    | Ngày 11–17 | ✅ Hoàn thành   |
| 06    | Polish + Docs + Submit                       | Ngày 18–21 | ✅ Hoàn thành   |

---

## Trạng thái ký hiệu

```bash
⬜ Chưa bắt đầu
🔄 Đang làm
✅ Hoàn thành
⏸️  Tạm dừng
```

---

## Phase 01 — Foundation

**File chi tiết:** `phase-01-foundation.md`

**Status: ✅ HOÀN THÀNH**

- [x] Định nghĩa `struct LogRecord` với 7 field
- [x] Định nghĩa `enum EventType` và `enum Location`
- [x] Viết `CsvLoader` — đọc và parse từng dòng CSV
- [x] Viết `Validator` — validate từng field, skip dòng lỗi
- [x] Viết `DataStore` — dynamic array thủ công với realloc
- [x] Viết `Deduplicator` — loại bỏ dòng trùng lặp hoàn toàn
- [x] Viết `StringUtils` — split, trim, compare
- [x] Viết `TimeUtils` — parse timestamp, format datetime
- [x] CLI skeleton: lệnh `load` và `exit`
- [x] Test: load 10k dòng thành công, không crash, không leak

---

## Phase 02 — Indexing

**File chi tiết:** `phase-02-indexing.md`

**Status: ✅ HOÀN THÀNH**

- [x] Viết `HashTable` thủ công (separate chaining với linked list)
- [x] `HashIndex` theo `user_id` → mảng LogRecord\*
- [x] `HashIndex` theo `device_id` → mảng LogRecord\*
- [x] `HashIndex` theo `resource_id` → mảng LogRecord\*
- [x] `SortedIndex` — mảng con trỏ sorted by timestamp (merge sort)
- [x] `IndexManager` — khởi tạo, rebuild, giải phóng tất cả index
- [x] Test: lookup user → đúng records, thời gian lookup O(1) avg
- [x] Test: binary search time range → correct indices
- [x] Valgrind: 0 memory leaks

---

## Phase 03 — Query Engine (Midterm)

**File chi tiết:** `phase-03-query-midterm.md`

**Status: ✅ HOÀN THÀNH**

- [x] `UserJourneyQuery` (FR-03): filter by user + time range
- [x] `ResourceJourneyQuery` (FR-04): filter by resource + time range
- [x] `TopResourceQuery` (FR-05): count map thủ công, top 10
- [x] `QueryEngine` — dispatcher nhận lệnh từ CLI
- [x] `Printer` — format và in kết quả dạng bảng
- [x] CLI: lệnh `query user`, `query resource`, `top resources`
- [x] Test: đúng kết quả, đúng thứ tự, edge case empty
- [x] **[ MIDTERM SUBMISSION ]** ← Ready

---

## Phase 04 — Scale Up

**File chi tiết:** `phase-04-scale-up.md`

**Status: ✅ HOÀN THÀNH**

- [x] Test load 1M dòng — không crash
- [x] Đo thời gian: load + index + query < 10s trên 1M dòng
- [x] Tối ưu hash function nếu collision nhiều (Đã tối ưu thuật toán Deduplicator về O(n log n))
- [x] Kiểm tra toàn bộ memory leak (manual hoặc valgrind)
- [x] Test với dữ liệu có nhiều invalid rows và duplicates
- [x] Stress test: 10 query liên tiếp, bộ nhớ ổn định

---

## Phase 05 — Anomaly Detection (Final)

**File chi tiết:** `phase-05-anomaly.md`

**Status: ✅ HOÀN THÀNH**

- [x] `SessionBuilder` — xây dựng session từ event stream (FR-08 prereq)
- [x] `ThresholdDetector`:
  - [x] A01: Multi-device login
  - [x] A02: Consecutive failed login
  - [x] A03: Resource flood per device
  - [x] A04: Off-hours access
- [x] `BehaviorDetector`:
  - [x] A05: Impossible travel
  - [x] A06: Location churning
- [x] `SessionDetector`:
  - [x] A07: Long session
  - [x] A08: Session flood
  - [x] A09: Dangerous sequence (ADMIN + DOWNLOAD)
- [x] `AnomalyEngine` — tổng hợp, dedup, format report
- [x] `AdvancedDetector` (bonus):
  - [x] A10: Brute force detection
  - [x] A11: Dormant user activation
  - [x] A12: Privilege escalation (extra bonus)
  - [x] A13: Data exfiltration (extra bonus)
  - [x] A14: Lateral movement (extra bonus)
- [x] CLI: lệnh `detect anomaly`
- [x] Giải phóng memory sau mỗi lần detect
- [x] **[ FINAL SUBMISSION ]**

---

## Phase 06 — Polish + Submit

**File chi tiết:** `phase-06-submit.md`

**Status: ✅ HOÀN THÀNH**

- [x] Code review toàn bộ: memory, edge cases, correctness
- [x] Refactor: meaningful names, no magic numbers, SRP
- [x] Build release mode — kiểm tra binary chạy được
- [x] Viết `doc/report.md` — báo cáo đánh giá mức hoàn thành
- [x] Viết `doc/system_design.md` — thiết kế hệ thống
- [x] Viết `doc/user_guide.md` — hướng dẫn sử dụng
- [x] Xóa thư mục Debug và file tạm
- [x] Scan virus
- [x] Nén thành `24120421.zip`
- [x] Nộp Moodle đúng hạn
