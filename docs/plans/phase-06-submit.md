# Phase 06 — Polish + Submit

**Thời lượng:** 3–4 ngày
**Prereq:** Phase 05 hoàn thành
**Mục tiêu:** Code sạch, binary chạy được, docs đầy đủ, nộp đúng hạn.

---

## Checklist

### Code Review
- [ ] Rà soát: mọi `new` đều có `delete` tương ứng
- [ ] Rà soát: không có magic number, dùng constant
- [ ] Rà soát: không có hàm quá dài (> 50 dòng → xem xét tách)
- [ ] Rà soát: mọi hàm nhận pointer đều check nullptr
- [ ] Compile với `-Wall -Wextra`, fix hết warning

### Build Release
- [ ] Build ở release mode (g++ -O2 hoặc MSVC Release)
- [ ] Test binary trong `release/` chạy được độc lập
- [ ] Binary hoạt động đúng với file 1M dòng

### Documentation
- [ ] `doc/report.pdf` — báo cáo gồm:
  - Mức độ hoàn thành từng yêu cầu
  - Thiết kế hệ thống (diagram nếu có)
  - Các khó khăn và hướng giải quyết
- [ ] `doc/user_guide.md` — hướng dẫn:
  - Cách build
  - Cách chạy
  - Danh sách lệnh CLI và ví dụ
- [ ] `README.md` — giới thiệu ngắn

### Cleanup
- [ ] Xóa thư mục `Debug/`
- [ ] Xóa `.vs/`, `*.o`, `*.obj`, các file tạm
- [ ] Giữ lại: `src/`, `release/`, `doc/`, `data/sample.csv`, `README.md`

### Submission
- [ ] Cấu trúc thư mục đúng: `24120421/src/`, `24120421/release/`, `24120421/doc/`
- [ ] Scan virus
- [ ] Nén thành `24120421.zip`
- [ ] Kiểm tra zip extract ra đúng cấu trúc
- [ ] Nộp lên Moodle trước deadline
- [ ] Chụp màn hình xác nhận nộp thành công

---

## Submission Checklist cuối cùng
```
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
