# AGENTS.md — Halo Cyber Access Engine

# File hiến pháp. Load file này trước mọi thứ.

## Tổng quan dự án

- **Tên:** Halo – Cyber Access Engine
- **Loại:** Đồ án môn Cơ Sở Lập Trình, cá nhân
- **Sinh viên:** 24120421 — HCMUS
- **Ngôn ngữ:** C++ (C++17), không dùng STL container
- **Môi trường build:** Visual Studio / g++ release mode
- **Tài liệu gốc:** `docs/brief.md`, `docs/BRD.md`

---

## Phong cách phản hồi của AI

1. **Luôn dùng tiếng Việt** trong giải thích và comment trong code.
2. **Code comment bằng tiếng Anh** — theo chuẩn industry.
3. Sau mỗi phase / plan hoàn thành, **luôn hướng dẫn manual test** cụ thể:
   - Input mẫu cần dùng
   - Expected output cần thấy
   - Cách xác nhận không có memory leak
4. Khi gặp task phức tạp mà user phải làm thủ công:
   - Ưu tiên đề xuất script tự động hóa nếu có thể
   - Ghi rõ từng bước nếu bắt buộc manual
5. **Luôn có walkthrough** sau khi hoàn thành một unit công việc:
   - Tóm tắt những gì đã thay đổi
   - Lý do kỹ thuật cho mỗi quyết định quan trọng
   - Những điểm cần chú ý / rủi ro tiềm ẩn
6. Khi đề xuất code, **ưu tiên theo thứ tự:**
   - Correctness → Memory safety → Performance → Readability

---

## Tech Stack

| Thành phần       | Lựa chọn        | Ghi chú                     |
| ---------------- | --------------- | --------------------------- |
| Ngôn ngữ         | C++17           | Bắt buộc                    |
| Build            | g++ hoặc MSVC   | Release mode cho submission |
| Cấu trúc dữ liệu | Tự cài đặt      | Không dùng vector, map, set |
| String           | std::string     | Được phép                   |
| File I/O         | fstream         | Được phép                   |
| Testing          | Manual + script | Xem `docs/plans/`           |

---

## Ràng buộc kỹ thuật — BẮT BUỘC

```
✅ PHẢI dùng: struct, con trỏ, dynamic array / linked list thủ công
❌ CẤM dùng: vector, map, unordered_map, set, thư viện đồ thị
⚠️  Mọi new[] đều phải có delete[] tương ứng
⚠️  Giải phóng bộ nhớ SAU MỖI chức năng, không đợi đến cuối
```

---

## Quy tắc code

- Tên biến, hàm: `camelCase` cho biến, `PascalCase` cho struct/class
- Tên file: `PascalCase.h` / `PascalCase.cpp`
- Mỗi hàm làm **đúng một việc** (Single Responsibility)
- Không dùng magic number — khai báo constant có tên
- Không dùng `goto`
- Mọi hàm nhận con trỏ phải kiểm tra `nullptr` đầu vào

---

## Trigger update CHANGELOG

AI **phải tự động cập nhật** `docs/CHANGELOG.md` khi:

- Hoàn thành một phase (dù một phần)
- Fix bug có ảnh hưởng đến behavior
- Refactor thay đổi interface của module
- Thêm/xóa file source

Format entry CHANGELOG:

```
## [YYYY-MM-DD] — Tên Phase / Mô tả ngắn
### Đã hoàn thành
- ✅ Mô tả cụ thể
### Ghi chú
- Những điểm đặc biệt, known issues
```

Đồng thời đánh dấu `[x]` vào `docs/plans/master-plan.md` cho phase tương ứng.

---

## Cấu trúc thư mục chuẩn

```
Halo-cyper-access-engine/
├── AGENTS.md               ← File này
├── src/
│   ├── main.cpp
│   ├── include/            ← Header files
│   │   ├── models/         ← Struct định nghĩa
│   │   ├── storage/        ← Load, parse, validate CSV
│   │   ├── indexing/       ← Hash table, sorted index
│   │   ├── query/          ← Query engine midterm
│   │   ├── anomaly/        ← Anomaly detection final
│   │   ├── session/        ← Session builder
│   │   ├── cli/            ← CLI handler, printer
│   │   └── utils/          ← Time, string, memory helpers
│   ├── source/             ← Source files
│   │   ├── models/
│   │   ├── storage/
│   │   ├── indexing/
│   │   ├── query/
│   │   ├── anomaly/
│   │   ├── session/
│   │   ├── cli/
│   │   └── utils/
├── docs/
│   ├── brief.md
│   ├── BRD.md
│   ├── CHANGELOG.md
│   └── plans/
│       ├── master-plan.md
│       ├── phase-01-*.md
│       └── ...
├── release/                ← Binary nộp bài
│   └── halo.exe
├── data/                   ← CSV test files
└── README.md
```

---

## Quy trình làm việc mỗi phase

```
1. Pick task   → Xem master-plan.md
2. Lên plan    → Tạo/cập nhật file phase-XX.md
3. Review plan → Xác nhận trước khi code
4. Thực thi    → AI code, có walkthrough
5. Test        → Manual test theo hướng dẫn trong phase file
6. Fix/improve → Nếu cần
7. Update docs → CHANGELOG.md + master-plan.md
```
