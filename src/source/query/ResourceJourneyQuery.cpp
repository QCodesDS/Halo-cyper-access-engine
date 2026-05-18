/**
 * @file        ResourceJourneyQuery.cpp
 * @brief       Implementation các hàm lọc dữ liệu, sắp xếp cục bộ và định dạng
 * hiển thị hành trình tài nguyên.
 */

#include "query/ResourceJourneyQuery.h"
#include "utils/TimeUtils.h"
#include <cstdio>

// ================================================================================
//  Helper functions (Nội bộ)
// ================================================================================

/**
 * @brief Thuật toán sắp xếp chèn (Insertion Sort) áp dụng cục bộ cho tập kết
 * quả nhỏ.
 * @param records Mảng các con trỏ bản ghi LogRecord cần sắp xếp ổn định
 * @param count Số lượng phần tử hiện tại có trong mảng
 */
static void insertionSortByTimestamp(LogRecord **records, int count) {
  for (int i = 1; i < count; i++) {
    LogRecord *key = records[i];
    int j = i - 1;

    while (j >= 0 && records[j]->timestamp > key->timestamp) {
      records[j + 1] = records[j];
      j--;
    }
    records[j + 1] = key;
  }
}

// ================================================================================
//  Public functions
// ================================================================================

void executeResourceJourney(const ResourceJourneyQuery &q,
                            const HashIndex &hashIdx, const SortedIndex &) {
  // Lấy ra danh sách toàn bộ các bản ghi liên quan đến mã tài nguyên này từ
  // bảng băm
  int resourceRecordCount = 0;
  LogRecord **resourceRecords =
      getByResource(hashIdx, q.resourceId, resourceRecordCount);

  // Con trỏ quản lý mảng lọc tạm thời
  LogRecord **filtered = nullptr;
  int filteredCount = 0;

  if (resourceRecordCount > 0) {
    // Cấp phát vùng nhớ động tạm thời cho mảng kết quả sau lọc
    filtered = new LogRecord *[resourceRecordCount];

    // Tiến hành lọc tuyến tính các bản ghi nằm trong khoảng thời gian
    // [timeStart, timeEnd]
    for (int i = 0; i < resourceRecordCount; i++) {
      if (resourceRecords[i]->timestamp >= q.timeStart &&
          resourceRecords[i]->timestamp <= q.timeEnd) {
        filtered[filteredCount++] = resourceRecords[i];
      }
    }
  }

  // Sắp xếp lại tập kết quả sau khi lọc theo thứ tự thời gian tăng dần
  if (filteredCount > 0) {
    insertionSortByTimestamp(filtered, filteredCount);
  }

  // Chuyển đổi định dạng timestamp Unix sang chuỗi ngày giờ đọc được (Readable
  // string)
  std::string startDate = epochToReadable(q.timeStart);
  std::string endDate = epochToReadable(q.timeEnd);

  // Trích xuất cấu trúc chuỗi chỉ lấy phần ngày (YYYY-MM-DD)
  std::string startDateOnly = startDate.substr(0, 10);
  std::string endDateOnly = endDate.substr(0, 10);

  // In thông tin tiêu đề truy vấn hành trình
  printf("Resource Journey: %s | %s to %s\n", q.resourceId.c_str(),
         startDateOnly.c_str(), endDateOnly.c_str());
  printf("─────────────────────────────────────────────────────────────────────"
         "──────────────\n");

  if (filteredCount == 0) {
    printf("[INFO] No events found for resource %s\n", q.resourceId.c_str());
  } else {
    // Duyệt và hiển thị chi tiết từng bản ghi log thỏa mãn điều kiện
    // Định dạng đầu ra: [YYYY-MM-DD HH:MM:SS] user_id → device_id → app_id
    // (event_type @ location)
    for (int i = 0; i < filteredCount; i++) {
      LogRecord *record = filtered[i];
      std::string timestamp = epochToReadable(record->timestamp);
      std::string eventStr = eventTypeToString(record->event_type);
      std::string locStr = locationToString(record->location);

      printf("[%s] %s → %s → %s (%s @ %s)\n", timestamp.c_str(),
             record->user_id.c_str(), record->device_id.c_str(),
             record->app_id.c_str(), eventStr.c_str(), locStr.c_str());
    }

    printf("───────────────────────────────────────────────────────────────────"
           "────────────────\n");
    printf("Total: %d events\n", filteredCount);
  }

  // Thu hồi bộ nhớ mảng tạm thời (Lưu ý: Không xóa các LogRecord vì quyền sở
  // hữu thuộc về DataStore)
  if (filtered != nullptr) {
    delete[] filtered;
    filtered = nullptr;
  }
}