# Phase 04 — Scale Up
# 1M Rows + Memory Audit

**Thời lượng:** 2 ngày
**Prereq:** Phase 03 hoàn thành
**Mục tiêu:** Hệ thống stable, nhanh, không leak trên 1M dòng.

---

## Checklist

### Performance
- [ ] Test load 1.000.000 dòng — không crash
- [ ] Đo: load time < 15s, query time < 5s, total < 10s
- [ ] Tối ưu DataStore capacity growth nếu cần
- [ ] Tối ưu hash table size cho 1M records

### Memory Audit
- [ ] Rà soát toàn bộ `new` có `delete` tương ứng
- [ ] Kiểm tra: sau `load` → `query` → `exit` không leak
- [ ] Kiểm tra: load nhiều file liên tiếp không tích lũy leak

### Robustness
- [ ] Test với file 1M dòng có 5% dòng invalid
- [ ] Test với file 1M dòng có 10% duplicates
- [ ] Test query trên tập rỗng sau khi filter

### Manual Test Guide
```
1. Sinh file 1M dòng (dùng script Python hoặc tool generate)
2. load data/large.csv → ghi lại thời gian
3. query user U001 0 9999999999 → ghi lại thời gian
4. top resources 0 9999999999 → ghi lại thời gian
5. exit → check không crash
```

---

## Definition of Done
- [ ] Load 1M dòng không crash
- [ ] Tổng thời gian xử lý < 10s trên 1M dòng
- [ ] Valgrind: no leak (hoặc manual verify)
- [ ] Update CHANGELOG + master-plan
