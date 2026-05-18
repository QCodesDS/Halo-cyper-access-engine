/**
 * @file        TopResourceQuery.cpp
 * @brief       Implementation giải thuật đếm tần suất, sắp xếp chọn (Selection
 * Sort) và hiển thị Top Resources.
 */

#include "query/TopResourceQuery.h"
#include "utils/TimeUtils.h"
#include <cstdio>

// ================================================================================
//  Helper functions (Nội bộ)
// ================================================================================

/**
 * @brief Tìm kiếm xem một mã tài nguyên đã tồn tại trong bảng ánh xạ đếm tần
 * suất hay chưa.
 * @param entries Mảng các cấu trúc thực thể đếm tần suất
 * @param count Số lượng phần tử hiện tại trong mảng entries
 * @param resourceId Mã tài nguyên cần đối chiếu
 * @return Chỉ mục vị trí nếu tìm thấy, ngược lại trả về -1
 */
static int findCountEntry(CountEntry *entries, int count,
                          const std::string &resourceId) {
  for (int i = 0; i < count; i++) {
    if (entries[i].resourceId == resourceId) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Thuật toán sắp xếp chọn (Selection Sort) để tìm ra N phần tử có tần
 * suất cao nhất (Giảm dần).
 * @param entries Mảng các cấu trúc thực thể đếm tần suất
 * @param count Tổng số lượng phần tử có trong mảng
 * @param topN Ngưỡng giới hạn số lượng phần tử cần sắp xếp tối đa
 */
static void selectionSortTopN(CountEntry *entries, int count, int topN) {
  int limit = (topN < count) ? topN : count;

  for (int i = 0; i < limit; i++) {
    int maxIdx = i;
    for (int j = i + 1; j < count; j++) {
      if (entries[j].count > entries[maxIdx].count) {
        maxIdx = j;
      }
    }

    // Hoán vị phần tử lớn nhất tìm thấy lên vị trí hiện tại
    CountEntry temp = entries[i];
    entries[i] = entries[maxIdx];
    entries[maxIdx] = temp;
  }
}

// ================================================================================
//  Public functions
// ================================================================================

void executeTopResources(long long timeStart, long long timeEnd,
                         const SortedIndex &sortedIdx, int topN) {
  // Khoanh vùng phạm vi các bản ghi bằng tìm kiếm nhị phân dựa trên mốc thời
  // gian
  int startIdx = binarySearchStart(sortedIdx, timeStart);
  int endIdx = binarySearchEnd(sortedIdx, timeEnd);

  // Khởi tạo các thông số quản lý mảng ánh xạ đếm tần suất (Count Map)
  CountEntry *countMap = nullptr;
  int mapSize = 0;
  int mapCapacity = 100; // Sức chứa khởi tạo ban đầu

  if (startIdx <= endIdx && startIdx < sortedIdx.count && endIdx >= 0) {
    countMap = new CountEntry[mapCapacity];

    // Duyệt tuyến tính qua phân vùng thời gian đã khoanh vùng để tích lũy số
    // lượng đếm
    for (int i = startIdx; i <= endIdx; i++) {
      LogRecord *record = sortedIdx.records[i];
      int entryIdx = findCountEntry(countMap, mapSize, record->resource_id);

      if (entryIdx >= 0) {
        // Tài nguyên đã có trong bản đồ -> Tăng số đếm lên 1
        countMap[entryIdx].count++;
      } else {
        // Tài nguyên mới xuất hiện -> Bổ sung vào mảng, tự động mở rộng bộ nhớ
        // nếu đầy
        if (mapSize >= mapCapacity) {
          CountEntry *newMap = new CountEntry[mapCapacity * 2];
          for (int k = 0; k < mapSize; k++) {
            newMap[k] = countMap[k];
          }
          delete[] countMap;
          countMap = newMap;
          mapCapacity *= 2;
        }
        countMap[mapSize].resourceId = record->resource_id;
        countMap[mapSize].count = 1;
        mapSize++;
      }
    }
  }

  // Tiến hành sắp xếp mảng tần suất theo thứ tự giảm dần để lấy ra Top N
  if (mapSize > 0) {
    selectionSortTopN(countMap, mapSize, topN);
  }

  // Chuyển đổi định dạng timestamp Unix sang chuỗi ngày giờ đọc được (Readable
  // string)
  std::string startDate = epochToReadable(timeStart);
  std::string endDate = epochToReadable(timeEnd);

  // Trích xuất lấy phần ngày (YYYY-MM-DD)
  std::string startDateOnly = startDate.substr(0, 10);
  std::string endDateOnly = endDate.substr(0, 10);

  // In thông tin tiêu đề kết quả thống kê tài nguyên phổ biến
  printf("Top %d Resources | %s to %s\n", topN, startDateOnly.c_str(),
         endDateOnly.c_str());
  printf("──────────────────────────────────────────\n");
  printf("Rank  Resource    Count\n");
  printf("──────────────────────────────────────────\n");

  if (mapSize == 0) {
    printf("[INFO] No resources found in time range\n");
  } else {
    int displayCount = (topN < mapSize) ? topN : mapSize;
    for (int i = 0; i < displayCount; i++) {
      printf("%-5d %-12s %d\n", i + 1, countMap[i].resourceId.c_str(),
             countMap[i].count);
    }
  }

  printf("──────────────────────────────────────────\n");

  // Giải phóng bộ nhớ động của bảng ánh xạ đếm tần suất tạm thời
  if (countMap != nullptr) {
    delete[] countMap;
    countMap = nullptr;
  }
}