/**
 * @file        Deduplicator.cpp
 * @brief       Implementation của module loại bỏ bản ghi trùng lặp.
 */

#include "storage/Deduplicator.h"
#include "storage/DataStore.h"
#include "utils/StringUtils.h"

// ================================================================================
//  Helper functions
// ================================================================================

bool recordsEqual(const LogRecord &r1, const LogRecord &r2) {
  return r1.timestamp == r2.timestamp && r1.event_type == r2.event_type &&
         r1.location == r2.location && strEquals(r1.user_id, r2.user_id) &&
         strEquals(r1.device_id, r2.device_id) &&
         strEquals(r1.app_id, r2.app_id) &&
         strEquals(r1.resource_id, r2.resource_id);
}

// ================================================================================
//  Merge Sort Helper for Deduplication
// ================================================================================

static void mergeStore(LogRecord **arr, int left, int mid, int right) {
  int leftSize = mid - left + 1;
  int rightSize = right - mid;

  LogRecord **leftArr = new LogRecord *[leftSize];
  LogRecord **rightArr = new LogRecord *[rightSize];

  for (int i = 0; i < leftSize; i++)
    leftArr[i] = arr[left + i];
  for (int j = 0; j < rightSize; j++)
    rightArr[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < leftSize && j < rightSize) {
    if (leftArr[i]->timestamp <= rightArr[j]->timestamp) {
      arr[k++] = leftArr[i++];
    } else {
      arr[k++] = rightArr[j++];
    }
  }
  while (i < leftSize)
    arr[k++] = leftArr[i++];
  while (j < rightSize)
    arr[k++] = rightArr[j++];

  delete[] leftArr;
  delete[] rightArr;
}

static void sortStoreByTimestamp(LogRecord **arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    sortStoreByTimestamp(arr, left, mid);
    sortStoreByTimestamp(arr, mid + 1, right);
    mergeStore(arr, left, mid, right);
  }
}

// ================================================================================
//  Main function
// ================================================================================

int deduplicate(DataStore &store) {
  if (store.count <= 1)
    return 0;

  // BƯỚC 1: Sắp xếp toàn bộ dữ liệu theo timestamp trước (O(N log N))
  sortStoreByTimestamp(store.records, 0, store.count - 1);

  // BƯỚC 2: Loại bỏ trùng lặp (O(N))
  int duplicatesRemoved = 0;

  // Tạo DataStore tạm để chứa các bản ghi unique
  DataStore tempStore;
  initDataStore(tempStore, store.capacity);

  // Thêm bản ghi đầu tiên
  LogRecord *firstCopy = new LogRecord(*store.records[0]);
  push(tempStore, firstCopy);

  // Kiểm tra từng bản ghi còn lại
  for (int i = 1; i < store.count; ++i) {
    bool isDuplicate = false;

    // Vì mảng đã sắp xếp theo timestamp, bản ghi trùng lặp chắc chắn sẽ
    // có cùng timestamp và nằm ở cuối của tempStore.
    // Chỉ cần tìm lùi lại các bản ghi có cùng timestamp.
    for (int j = tempStore.count - 1; j >= 0; --j) {
      if (tempStore.records[j]->timestamp != store.records[i]->timestamp) {
        // Khác timestamp thì chắc chắn không trùng, và không cần tìm sâu hơn.
        break;
      }
      if (recordsEqual(*store.records[i], *tempStore.records[j])) {
        isDuplicate = true;
        ++duplicatesRemoved;
        break;
      }
    }

    // Nếu chưa tồn tại thì thêm vào
    if (!isDuplicate) {
      LogRecord *copy = new LogRecord(*store.records[i]);
      push(tempStore, copy);
    }
  }

  // Giải phóng bộ nhớ của store cũ
  for (int i = 0; i < store.count; ++i) {
    delete store.records[i];
  }
  delete[] store.records;

  // Thay thế bằng dữ liệu đã deduplicate
  store.records = tempStore.records;
  store.count = tempStore.count;
  store.capacity = tempStore.capacity;

  return duplicatesRemoved;
}