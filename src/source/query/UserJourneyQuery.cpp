/**
 * @file        UserJourneyQuery.cpp
 * @brief       Implementation các thuật toán lọc hành vi, sắp xếp cục bộ và định dạng hiển thị hành trình người dùng.
 */

#include "query/UserJourneyQuery.h"
#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"
#include <cstdio>
#include <cstring>

// ================================================================================
//  Helper functions (Nội bộ)
// ================================================================================

/**
 * @brief Thuật toán sắp xếp chèn (Insertion Sort) áp dụng cục bộ cho tập kết quả hành trình.
 * @param records Mảng các con trỏ bản ghi LogRecord cần sắp xếp ổn định
 * @param count Số lượng phần tử hiện tại có trong mảng
 */
static void insertionSortByTimestamp(LogRecord **records, int count)
{
    for (int i = 1; i < count; i++)
    {
        LogRecord *key = records[i];
        int j = i - 1;
        
        while (j >= 0 && records[j]->timestamp > key->timestamp)
        {
            records[j + 1] = records[j];
            j--;
        }
        records[j + 1] = key;
    }
}

// ================================================================================
//  Public functions
// ================================================================================

void executeUserJourney(const UserJourneyQuery &q, const HashIndex &hashIdx, const SortedIndex &)
{
    // Truy cập nhanh toàn bộ các bản ghi của người dùng cụ thể này từ bảng băm chỉ mục
    int userRecordCount = 0;
    LogRecord **userRecords = getByUser(hashIdx, q.userId, userRecordCount);

    // Cấp phát mảng tạm thời lọc kết quả
    LogRecord **filtered = nullptr;
    int filteredCount = 0;

    if (userRecordCount > 0)
    {
        filtered = new LogRecord *[userRecordCount];

        // Lọc tuyến tính lấy các bản ghi nằm trong khoảng [timeStart, timeEnd]
        for (int i = 0; i < userRecordCount; i++)
        {
            if (userRecords[i]->timestamp >= q.timeStart && userRecords[i]->timestamp <= q.timeEnd)
            {
                filtered[filteredCount++] = userRecords[i];
            }
        }
    }

    // Sắp xếp lại tập kết quả sau khi lọc theo thứ tự thời gian tăng dần
    if (filteredCount > 0)
    {
        insertionSortByTimestamp(filtered, filteredCount);
    }

    // Chuyển đổi định dạng timestamp Unix sang chuỗi ngày giờ đọc được (Readable string)
    std::string startDate = epochToReadable(q.timeStart);
    std::string endDate = epochToReadable(q.timeEnd);

    // Trích xuất lấy phần ngày (YYYY-MM-DD)
    std::string startDateOnly = startDate.substr(0, 10);
    std::string endDateOnly = endDate.substr(0, 10);

    // In thông tin tiêu đề truy vấn hành trình người dùng
    printf("User Journey: %s | %s to %s\n", q.userId.c_str(), startDateOnly.c_str(), endDateOnly.c_str());
    printf("─────────────────────────────────────────────────────────────────────────────────\n");

    if (filteredCount == 0)
    {
        printf("[INFO] No events found for user %s\n", q.userId.c_str());
    }
    else
    {
        // Duyệt hiển thị chuỗi hành trình cụ thể của người dùng
        // Định dạng đầu ra: [YYYY-MM-DD HH:MM:SS] device_id → app_id → resource_id (event_type @ location)
        for (int i = 0; i < filteredCount; i++)
        {
            LogRecord *record = filtered[i];
            std::string timestamp = epochToReadable(record->timestamp);
            std::string eventStr = eventTypeToString(record->event_type);
            std::string locStr = locationToString(record->location);

            printf("[%s] %s → %s → %s (%s @ %s)\n",
                   timestamp.c_str(),
                   record->device_id.c_str(),
                   record->app_id.c_str(),
                   record->resource_id.c_str(),
                   eventStr.c_str(),
                   locStr.c_str());
        }

        printf("─────────────────────────────────────────────────────────────────────────────────\n");
        printf("Total: %d events\n", filteredCount);
    }

    // Thu hồi bộ nhớ mảng tạm (Lưu ý: Không xóa các LogRecord vì quyền sở hữu thuộc về DataStore)
    if (filtered != nullptr)
    {
        delete[] filtered;
        filtered = nullptr;
    }
}