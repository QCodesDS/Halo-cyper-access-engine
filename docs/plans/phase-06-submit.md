# Phase 06 — Polish + Submit

**Thời lượng:** 3–4 ngày
**Prereq:** Phase 05 hoàn thành
**Mục tiêu:** Code sạch, binary chạy được, docs đầy đủ, nộp đúng hạn.

---

## Checklist

### Code Review

- [x] Rà soát: mọi `new` đều có `delete` tương ứng
- [x] Rà soát: không có magic number, dùng constant
- [x] Rà soát: không có hàm quá dài (> 50 dòng → xem xét tách)
- [x] Rà soát: mọi hàm nhận pointer đều check nullptr
- [x] Compile với `-Wall -Wextra`, fix hết warning

### Build Release

- [x] Build ở release mode (g++ -O2 hoặc MSVC Release)
- [x] Test binary trong `release/` chạy được độc lập
- [x] Binary hoạt động đúng với file 1M dòng

### Documentation

- [x] `doc/report.md` — báo cáo gồm:
  - Mức độ hoàn thành từng yêu cầu
  - Thiết kế hệ thống (diagram nếu có)
  - Các khó khăn và hướng giải quyết
- [x] `doc/user_guide.md` — hướng dẫn:
  - Cách build
  - Cách chạy
  - Danh sách lệnh CLI và ví dụ
- [x] `README.md` — giới thiệu ngắn

### Cleanup

- [x] Xóa thư mục `Debug/`
- [x] Xóa `.vs/`, `*.o`, `*.obj`, các file tạm
- [x] Giữ lại: `src/`, `release/`, `doc/`, `data/sample.csv`, `README.md`

### Submission

- [x] Cấu trúc thư mục đúng: `24120421/src/`, `24120421/release/`, `24120421/doc/`
- [x] Scan virus
- [x] Nén thành `24120421.zip`
- [x] Kiểm tra zip extract ra đúng cấu trúc
- [x] Nộp lên Moodle trước deadline
- [x] Chụp màn hình xác nhận nộp thành công

---

## Submission Checklist cuối cùng

```bash
24120421.zip
└── 24120421/
    ├── src/          ✅ toàn bộ source
    ├── release/      ✅ binary đã build
    ├── doc/          ✅ report + user guide
    └── README.md     ✅
```

**KHÔNG có trong zip:**

- [ ] Thư mục Debug/
- [ ] File .obj, .o, .pdb
- [ ] Thư mục .vs/ hoặc .vscode/
- [ ] File dữ liệu lớn (> 50MB)
