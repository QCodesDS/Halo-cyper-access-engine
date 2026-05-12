# Master Plan — Halo Cyber Access Engine

## Trạng thái tổng quan

| Phase | Tên                                          | Deadline   | Trạng thái      |
| ----- | -------------------------------------------- | ---------- | --------------- |
| 01    | Foundation — Models + Storage + CLI skeleton | Ngày 1–3   | ⬜ Chưa bắt đầu |
| 02    | Indexing — Hash Table + Sorted Index         | Ngày 4–5   | ⬜ Chưa bắt đầu |
| 03    | Query Engine (Midterm)                       | Ngày 6–8   | ⬜ Chưa bắt đầu |
| 04    | Scale Up — 1M rows + Memory audit            | Ngày 9–10  | ⬜ Chưa bắt đầu |
| 05    | Anomaly Detection (Final)                    | Ngày 11–17 | ⬜ Chưa bắt đầu |
| 06    | Polish + Docs + Submit                       | Ngày 18–21 | ⬜ Chưa bắt đầu |

---

## Trạng thái ký hiệu

```
⬜ Chưa bắt đầu
🔄 Đang làm
✅ Hoàn thành
⏸️  Tạm dừng
```

---

## Phase 01 — Foundation

**File chi tiết:** `phase-01-foundation.md`

- [ ] Định nghĩa `struct LogRecord` với 7 field
- [ ] Định nghĩa `enum EventType` và `enum Location`
- [ ] Viết `CsvLoader` — đọc và parse từng dòng CSV
- [ ] Viết `Validator` — validate từng field, skip dòng lỗi
- [ ] Viết `DataStore` — dynamic array thủ công với realloc
- [ ] Viết `Deduplicator` — loại bỏ dòng trùng lặp hoàn toàn
- [ ] Viết `StringUtils` — split, trim, compare
- [ ] Viết `TimeUtils` — parse timestamp, format datetime
- [ ] CLI skeleton: lệnh `load` và `exit`
- [ ] Test: load 10k dòng thành công, không crash, không leak

---

## Phase 02 — Indexing

**File chi tiết:** `phase-02-indexing.md`

- [ ] Viết `HashTable` thủ công (separate chaining với linked list)
- [ ] `HashIndex` theo `user_id` → mảng LogRecord\*
- [ ] `HashIndex` theo `device_id` → mảng LogRecord\*
- [ ] `HashIndex` theo `resource_id` → mảng LogRecord\*
- [ ] `SortedIndex` — mảng con trỏ sorted by timestamp (merge sort)
- [ ] `IndexManager` — khởi tạo, rebuild, giải phóng tất cả index
- [ ] Test: lookup user → đúng records, thời gian lookup O(1) avg

---

## Phase 03 — Query Engine (Midterm)

**File chi tiết:** `phase-03-query-midterm.md`

- [ ] `UserJourneyQuery` (FR-03): filter by user + time range
- [ ] `ResourceJourneyQuery` (FR-04): filter by resource + time range
- [ ] `TopResourceQuery` (FR-05): count map thủ công, top 10
- [ ] `QueryEngine` — dispatcher nhận lệnh từ CLI
- [ ] `Printer` — format và in kết quả dạng bảng
- [ ] CLI: lệnh `query user`, `query resource`, `top resources`
- [ ] Test: đúng kết quả, đúng thứ tự, edge case empty
- [ ] **[ MIDTERM SUBMISSION ]**

---

## Phase 04 — Scale Up

**File chi tiết:** `phase-04-scale-up.md`

- [ ] Test load 1M dòng — không crash
- [ ] Đo thời gian: load + index + query < 10s trên 1M dòng
- [ ] Tối ưu hash function nếu collision nhiều
- [ ] Kiểm tra toàn bộ memory leak (manual hoặc valgrind)
- [ ] Test với dữ liệu có nhiều invalid rows và duplicates
- [ ] Stress test: 10 query liên tiếp, bộ nhớ ổn định

---

## Phase 05 — Anomaly Detection (Final)

**File chi tiết:** `phase-05-anomaly.md`

- [ ] `SessionBuilder` — xây dựng session từ event stream (FR-08 prereq)
- [ ] `ThresholdDetector`:
  - [ ] A01: Multi-device login
  - [ ] A02: Consecutive failed login
  - [ ] A03: Resource flood per device
  - [ ] A04: Off-hours access
- [ ] `BehaviorDetector`:
  - [ ] A05: Impossible travel
  - [ ] A06: Location churning
- [ ] `SessionDetector`:
  - [ ] A07: Long session
  - [ ] A08: Session flood
  - [ ] A09: Dangerous sequence (ADMIN + DOWNLOAD)
- [ ] `AnomalyEngine` — tổng hợp, dedup, format report
- [ ] `AdvancedDetector` (bonus):
  - [ ] A10: Brute force detection
  - [ ] A11: Dormant user activation
- [ ] CLI: lệnh `detect anomaly`
- [ ] Giải phóng memory sau mỗi lần detect
- [ ] **[ FINAL SUBMISSION ]**

---

## Phase 06 — Polish + Submit

**File chi tiết:** `phase-06-submit.md`

- [ ] Code review toàn bộ: memory, edge cases, correctness
- [ ] Refactor: meaningful names, no magic numbers, SRP
- [ ] Build release mode — kiểm tra binary chạy được
- [ ] Viết `doc/report.pdf` — báo cáo đánh giá mức hoàn thành
- [ ] Viết `doc/system_design.md` — thiết kế hệ thống
- [ ] Viết `doc/user_guide.md` — hướng dẫn sử dụng
- [ ] Xóa thư mục Debug và file tạm
- [ ] Scan virus
- [ ] Nén thành `24120421.zip`
- [ ] Nộp Moodle đúng hạn
