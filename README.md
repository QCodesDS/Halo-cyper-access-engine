# Halo – Cyber Access Engine

Đồ án môn Cơ Sở Lập Trình — HCMUS — 24120421

## Mô tả ngắn

Engine phân tích log truy cập, phát hiện hành vi bất thường.

## Build & Run

```bash
# Bước 1: Cấu hình và sinh các file build vào thư mục 'build'

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Bước 2: Tiến hành biên dịch chương trình
cmake --build build --config Release

# Bước 3: Thực thi chương trình
./release/bin/halo
```

## Cấu trúc thư mục

src/ — source code
docs/ — tài liệu thiết kế
release/ — binary đã build
