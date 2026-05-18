/**
 * @file        UserJourneyQuery.h
 * @brief       Định nghĩa cấu trúc và hàm thực thi truy vấn dòng hành trình của
 * người dùng (User Journey).
 *
 * Hỗ trợ lọc ra tất cả hành vi tương tác, truy cập thiết bị và ứng dụng của một
 * người dùng cụ thể trong một khoảng thời gian nhất định, sắp xếp tuần tự theo
 * thời gian xảy ra.
 */

#ifndef USER_JOURNEY_QUERY_H
#define USER_JOURNEY_QUERY_H

#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

/**
 * @struct UserJourneyQuery
 * @brief  Cấu trúc chứa các tham số thiết lập đầu vào để thực thi truy vấn hành
 * trình người dùng.
 */
struct UserJourneyQuery {
  std::string
      userId; ///< Mã định danh duy nhất của người dùng cần điều tra hành trình
  long long timeStart; ///< Mốc thời gian bắt đầu khoảng lọc (Unix Timestamp)
  long long timeEnd;   ///< Mốc thời gian kết thúc khoảng lọc (Unix Timestamp)
};

/**
 * @brief  Thực thi truy vấn dòng hành trình người dùng và định dạng hiển thị
 * kết quả ra màn hình.
 * @note   Hàm định vị dữ liệu của người dùng thông qua chỉ mục băm
 * (`HashIndex`), lọc theo khoảng thời gian và thực hiện sắp xếp chèn (Insertion
 * Sort) cục bộ để đảm bảo thứ tự thời gian.
 * @param  q Cấu trúc chứa các tham số thiết lập truy vấn hành trình người dùng
 * @param  hashIdx Hệ thống chỉ mục băm đa chiều phục vụ truy xuất nhanh theo
 * khóa User ID
 * @param  sortedIdx Chỉ mục mảng đã sắp xếp (không sử dụng trực tiếp trong hàm
 * này)
 */
void executeUserJourney(const UserJourneyQuery &q, const HashIndex &hashIdx,
                        const SortedIndex &sortedIdx);

#endif // USER_JOURNEY_QUERY_H