# Phase 05 — Anomaly Detection (Final)

# Threshold + Behavior + Session + Advanced

**Thời lượng:** 7 ngày  
**Prereq:** Phase 04 hoàn thành  
**Mục tiêu:** Phát hiện đầy đủ anomaly FR-06 đến FR-09. Nộp bài final.

---

## Checklist

### Session Builder (prereq cho SessionDetector)

- [x] `src/session/SessionBuilder.h/.cpp`
  - Group events theo user_id, sort by timestamp
  - Session bắt đầu: LOGIN hoặc event đầu tiên của chuỗi
  - Session kết thúc: LOGOUT hoặc gap > 30 phút
  - Output: mảng `Session*` mỗi user

### Threshold Detector

- [x] `src/anomaly/ThresholdDetector.h/.cpp`
  - [x] A01: user dùng > 3 device khác nhau trong 60 phút
  - [x] A02: user có > 5 FAILED_LOGIN liên tiếp (không xen event thành công)
  - [x] A03: device truy cập > 20 resource khác nhau trong 30 phút
  - [x] A04: event ngoài 08:00–18:00 UTC

### Behavior Detector

- [x] `src/anomaly/BehaviorDetector.h/.cpp`
  - [x] A05: user xuất hiện 2+ location khác nhau trong 120 phút
  - [x] A06: user đổi location > 3 lần trong 60 phút

### Session Detector

- [x] `src/anomaly/SessionDetector.h/.cpp`
  - [x] A07: phiên > 8 tiếng liên tục
  - [x] A08: user tạo > 5 phiên trong 60 phút
  - [x] A09: trong 1 phiên có ADMIN_ACTION → DOWNLOAD trong 10 phút

### Anomaly Engine

- [x] `src/anomaly/AnomalyEngine.h/.cpp`
  - Gọi tất cả detector, tổng hợp kết quả
  - Dedup: cùng user + cùng loại anomaly + cùng time window → 1 entry
  - Format report: user_id | anomaly_type | timestamp | detail
  - Giải phóng toàn bộ sau khi in xong

### Advanced Detector (Bonus)

- [x] `src/anomaly/AdvancedDetector.h/.cpp`
  - [x] A10: ≥ 5 FAILED_LOGIN liên tiếp → cuối cùng LOGIN thành công
  - [x] A11: user không activity > 7 ngày → đột ngột > 20 event trong 1 giờ
  - [x] A12: privilege escalation detection (extra bonus)
  - [x] A13: data exfiltration detection (extra bonus)
  - [x] A14: lateral movement detection (extra bonus)

### CLI

- [x] `detect anomaly` — chạy tất cả
- [x] `detect anomaly --type=threshold`
- [x] `detect anomaly --type=behavior`
- [x] `detect anomaly --type=session`
- [x] `detect anomaly --type=advanced`

---

## Expected Output

```bash
> detect anomaly
[INFO] Running anomaly detection on 1,000,000 events...

=== ANOMALY REPORT ===
──────────────────────────────────────────────────────────────
Type                 User    Timestamp           Detail
──────────────────────────────────────────────────────────────
MULTI_DEVICE_LOGIN   U007    2024-04-15 10:31    4 devices in 45 min
IMPOSSIBLE_TRAVEL    U007    2024-04-15 14:22    VN → US in 12 min
DANGEROUS_SEQUENCE   U023    2024-04-16 02:11    ADMIN_ACTION → DOWNLOAD (3 min)
OFF_HOURS_ACCESS     U041    2024-04-16 03:45    Access at 03:45 UTC
BRUTE_FORCE          U012    2024-04-17 09:12    8 fails → success [BONUS]
──────────────────────────────────────────────────────────────
Total anomalies: 5 | Detection time: 2.34s
[INFO] Memory freed.
```

---

## Manual Test Guide

### Test A01: Multi-device

```bash
1. Inject 4 events: cùng user, 4 device khác nhau, trong 30 phút
2. detect anomaly --type=threshold
3. Expect: MULTI_DEVICE_LOGIN xuất hiện cho user đó
```

### Test A05: Impossible travel

```bash
1. Inject: U999, VN, timestamp T
2. Inject: U999, US, timestamp T + 3600 (1 tiếng sau)
3. Expect: IMPOSSIBLE_TRAVEL flagged (VN → US không thể trong 1h)
```

### Test A09: Dangerous sequence

```bash
1. Inject: U999, ADMIN_ACTION, timestamp T
2. Inject: U999, DOWNLOAD, timestamp T + 300 (5 phút sau)
3. Trong cùng session
4. Expect: DANGEROUS_SEQUENCE flagged
```

### Test Memory

```bash
1. detect anomaly (chạy toàn bộ)
2. detect anomaly (chạy lần 2)
3. Memory usage phải ổn định, không tăng
```

---

## Ngưỡng cấu hình (Constants)

```cpp
// src/anomaly/AnomalyConfig.h
const int    MAX_DEVICES_PER_HOUR    = 3;
const int    MAX_FAILED_LOGIN_STREAK = 5;
const int    MAX_RESOURCES_PER_30MIN = 20;
const int    WORK_HOUR_START         = 8;   // UTC
const int    WORK_HOUR_END           = 18;  // UTC
const int    IMPOSSIBLE_TRAVEL_MINS  = 120;
const int    MAX_LOCATION_CHANGES    = 3;
const int    MAX_SESSION_HOURS       = 8;
const int    MAX_SESSIONS_PER_HOUR   = 5;
const int    DANGEROUS_SEQ_MINS      = 10;
const int    SESSION_TIMEOUT_MINS    = 30;
const int    DORMANT_DAYS            = 7;
const int    BURST_EVENTS_PER_HOUR   = 20;
```

---

## Definition of Done

- [x] Tất cả A01–A09 detect đúng với test cases
- [x] A10, A11 hoạt động (bonus)
- [x] Memory giải phóng sau mỗi lần detect
- [x] Thời gian detect < 10s trên 1M records
- [x] Update CHANGELOG + master-plan
- [x] **FINAL SUBMITTED ✅**
