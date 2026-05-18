/**
 * @file        DataStore.cpp
 * @brief       Implementation của DataStore - container lưu trữ LogRecord.
 */

#include "storage/DataStore.h"

// ================================================================================
//  Implementation
// ================================================================================

DataStore::DataStore() : records(nullptr), capacity(0), count(0) {}

void initDataStore(DataStore &store, int initialCapacity) {
  if (initialCapacity < 1)
    initialCapacity = 1000;

  store.capacity = initialCapacity;
  store.count = 0;
  store.records = new LogRecord *[store.capacity];
}

void push(DataStore &store, LogRecord *r) {
  if (r == nullptr)
    return;

  // Mở rộng dung lượng nếu cần (doubling)
  if (store.count >= store.capacity) {
    int newCapacity = store.capacity * 2;
    if (newCapacity < 1000)
      newCapacity = 1000;

    LogRecord **newRecords = new LogRecord *[newCapacity];

    // Sao chép con trỏ
    for (int i = 0; i < store.count; ++i) {
      newRecords[i] = store.records[i];
    }

    delete[] store.records;

    store.records = newRecords;
    store.capacity = newCapacity;
  }

  store.records[store.count++] = r;
}

LogRecord *get(const DataStore &store, int i) {
  if (i < 0 || i >= store.count)
    return nullptr;

  return store.records[i];
}

int size(const DataStore &store) { return store.count; }

void clear(DataStore &store) {
  if (store.records != nullptr) {
    // Giải phóng từng LogRecord (vì DataStore sở hữu chúng)
    for (int i = 0; i < store.count; ++i) {
      delete store.records[i];
    }

    delete[] store.records;
  }

  // Reset về trạng thái ban đầu
  store.records = nullptr;
  store.count = 0;
  store.capacity = 0;
}

LogRecord **getAll(const DataStore &store) { return store.records; }