/**
 * @file        HashTable.cpp
 * @brief       Implementation các thuật toán băm cuốn, xử lý va chạm và quản lý
 * bộ nhớ của bảng băm.
 */

#include "indexing/HashTable.h"

// ================================================================================
//  Public functions
// ================================================================================

int hashFunction(const std::string &key, int tableSize) {
  if (tableSize <= 0)
    return 0;

  long long hash = 0;
  const long long prime = 31;
  const long long modulo = 1e9 + 7;

  for (char c : key) {
    hash = (hash * prime + (int)c) % modulo;
  }

  return (int)(hash % tableSize);
}

void initHashTable(HashTable &ht, int tableSize) {
  ht.tableSize = tableSize;
  ht.totalKeys = 0;
  ht.buckets = new HashNode *[tableSize];

  for (int i = 0; i < tableSize; i++) {
    ht.buckets[i] = nullptr;
  }
}

void insertHash(HashTable &ht, const std::string &key, LogRecord *record) {
  if (record == nullptr)
    return;

  int bucket = hashFunction(key, ht.tableSize);
  HashNode *node = ht.buckets[bucket];
  HashNode *prevNode = nullptr;

  // Tìm kiếm nút đã tồn tại khóa trùng khớp trong chuỗi va chạm (chain)
  while (node != nullptr) {
    if (node->key == key) {
      // Trùng khóa: Tiến hành bổ sung con trỏ log vào mảng động giá trị
      // (values)
      if (node->count == node->capacity) {
        // Tái cấp phát tăng gấp đôi kích thước mảng động khi đầy
        int newCapacity = node->capacity * 2;
        LogRecord **newValues = new LogRecord *[newCapacity];

        for (int i = 0; i < node->count; i++) {
          newValues[i] = node->values[i];
        }

        delete[] node->values;
        node->values = newValues;
        node->capacity = newCapacity;
      }

      node->values[node->count] = record;
      node->count++;
      return;
    }

    prevNode = node;
    node = node->next;
  }

  // Không tìm thấy khóa sẵn có: Khởi tạo nút mới và chèn vào danh sách liên kết
  HashNode *newNode = new HashNode();
  newNode->key = key;
  newNode->capacity = 10;
  newNode->count = 1;
  newNode->values = new LogRecord *[newNode->capacity];
  newNode->values[0] = record;
  newNode->next = nullptr;

  if (prevNode == nullptr) {
    // Trường hợp bucket đang trống (Nút đầu tiên của chuỗi)
    ht.buckets[bucket] = newNode;
  } else {
    // Chèn nối đuôi vào cuối chuỗi liên kết va chạm
    prevNode->next = newNode;
  }

  ht.totalKeys++;
}

LogRecord **lookupHash(const HashTable &ht, const std::string &key,
                       int &outCount) {
  outCount = 0;

  int bucket = hashFunction(key, ht.tableSize);
  HashNode *node = ht.buckets[bucket];

  // Duyệt dọc theo chuỗi liên kết va chạm để đối chiếu khóa tìm kiếm
  while (node != nullptr) {
    if (node->key == key) {
      outCount = node->count;
      return node->values;
    }

    node = node->next;
  }

  // Không tìm thấy khóa phù hợp trong toàn bộ chuỗi va chạm
  return nullptr;
}

void clearHashTable(HashTable &ht) {
  for (int i = 0; i < ht.tableSize; i++) {
    HashNode *node = ht.buckets[i];

    while (node != nullptr) {
      HashNode *temp = node;
      node = node->next;

      delete[] temp->values;
      delete temp;
    }
  }

  delete[] ht.buckets;
  ht.buckets = nullptr;
  ht.tableSize = 0;
  ht.totalKeys = 0;
}