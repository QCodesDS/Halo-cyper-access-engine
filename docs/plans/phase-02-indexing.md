# Phase 02 — Indexing
# Hash Table + Sorted Index

**Thời lượng:** 2 ngày
**Prereq:** Phase 01 hoàn thành
**Mục tiêu:** Lookup O(1) average theo user/device/resource. Query theo time range O(log n).

---

## Checklist

### Hash Table (tự cài đặt)
- [ ] `src/indexing/HashTable.h/.cpp`
  - Separate chaining: mỗi bucket là một linked list
  - Hash function: polynomial rolling hash cho string key
  - `insert(key, value*)` — thêm con trỏ vào bucket tương ứng
  - `lookup(key)` → trả về linked list các value*
  - `clear()` — giải phóng toàn bộ

### Hash Index
- [ ] `src/indexing/HashIndex.h/.cpp`
  - `buildByUser(DataStore& store)` — index: user_id → [LogRecord*]
  - `buildByDevice(DataStore& store)` — index: device_id → [LogRecord*]
  - `buildByResource(DataStore& store)` — index: resource_id → [LogRecord*]
  - `getByUser(const string& uid)` → array of LogRecord*
  - `clear()` — giải phóng index

### Sorted Index
- [ ] `src/indexing/SortedIndex.h/.cpp`
  - Mảng LogRecord* được sắp xếp theo `timestamp` tăng dần
  - Dùng merge sort (stable, O(n log n))
  - `binarySearchStart(long long t)` → int index (first ≥ t)
  - `binarySearchEnd(long long t)` → int index (last ≤ t)
  - Cho phép query range [t_start, t_end] trong O(log n + k)

### Index Manager
- [ ] `src/indexing/IndexManager.h/.cpp`
  - `buildAll(DataStore& store)` — khởi tạo tất cả index
  - `rebuildAll(DataStore& store)` — sau khi load file mới
  - `clearAll()` — giải phóng toàn bộ

---

## Expected Output

```
> load data/sample.csv
[INFO] Loaded 9847 records.
[INFO] Building indexes...
[INFO] Hash index (user): done
[INFO] Hash index (device): done
[INFO] Hash index (resource): done
[INFO] Sorted index (timestamp): done
[INFO] Index ready in 0.12s
```

---

## Manual Test Guide

### Test 1: Lookup user
```
1. Load file
2. Trong code: gọi hashIndex.getByUser("U007")
3. In ra tất cả record của U007
4. Expect: tất cả records có user_id == "U007"
```

### Test 2: Time range query
```
1. Dùng sortedIndex.binarySearchStart(1713000000)
2. Dùng sortedIndex.binarySearchEnd(1713999999)
3. Expect: chỉ lấy records trong khoảng timestamp đó
```

### Test 3: Hash collision handling
```
1. Insert nhiều keys khác nhau nhưng cùng bucket
2. Expect: tất cả đều lookup được đúng
```

### Test 4: Memory sau khi clearAll()
```
1. buildAll() → clearAll()
2. Expect: không leak (valgrind clean)
```

---

## Kỹ thuật quan trọng

### Linked List cho Hash Bucket
```cpp
struct Node {
    LogRecord* record;
    Node* next;
};

struct Bucket {
    Node* head;
};
```

### Hash Function cho string
```cpp
// Polynomial rolling hash — phân phối tốt
int hashString(const string& key, int tableSize) {
    const int BASE = 31;
    long long hash = 0;
    for (char c : key)
        hash = (hash * BASE + c) % tableSize;
    return (int)hash;
}
```

### Merge Sort trên mảng con trỏ
```cpp
// Sort mảng LogRecord** theo field timestamp
void mergeSort(LogRecord** arr, int left, int right);
void merge(LogRecord** arr, int l, int m, int r);
```

### Table Size
```
Dùng prime number gần nhất với 2x số record
Ví dụ: 10k records → table size 20011
        1M records → table size 1999993
```

---

## Definition of Done

- [ ] Lookup user/device/resource trả về đúng records
- [ ] Binary search trên sorted index đúng
- [ ] Không memory leak sau clearAll()
- [ ] Build time index < 5s trên 1M records
- [ ] Update CHANGELOG + master-plan
