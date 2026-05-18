/**
 * @file        ResourceJourneyQuery.h
 * @brief       Định nghĩa cấu trúc và hàm thực thi truy vấn hành trình phân
 * phối tài nguyên (Resource Journey).
 *
 * Cung cấp giải pháp tìm kiếm, lọc theo khoảng thời gian và hiển thị lịch sử
 * tương tác của tất cả người dùng/thiết bị đối với một tài nguyên hệ thống
 * (Resource) cụ thể.
 */

#ifndef RESOURCE_JOURNEY_QUERY_H
#define RESOURCE_JOURNEY_QUERY_H

#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

/**
 * @struct ResourceJourneyQuery
 * @brief  Cấu trúc chứa các tham số đầu vào phục vụ việc truy vấn hành trình
 * tài nguyên.
 */
struct ResourceJourneyQuery {
  std::string resourceId; ///< Mã định danh duy nhất của tài nguyên cần truy vấn
  long long timeStart;    ///< Mốc thời gian bắt đầu khoảng lọc (Unix Timestamp)
  long long timeEnd; ///< Mốc thời gian kết thúc khoảng lọc (Unix Timestamp)
};

/**
 * @brief  Thực thi truy vấn hành trình tài nguyên và in kết quả ra màn hình
 * console.
 * @note   Hàm trích xuất dữ liệu từ chỉ mục băm (`HashIndex`) theo chiều
 * Resource, sau đó lọc theo khoảng thời gian và sắp xếp lại bằng thuật toán
 * Insertion Sort trước khi hiển thị.
 * @param  q Cấu trúc chứa các tham số thiết lập truy vấn
 * @param  hashIdx Hệ thống chỉ mục băm đa chiều phục vụ tìm kiếm nhanh theo
 * khóa tài nguyên
 * @param  sortedIdx Chỉ mục mảng đã sắp xếp (không sử dụng trực tiếp trong hàm
 * này)
 */
void executeResourceJourney(const ResourceJourneyQuery &q,
                            const HashIndex &hashIdx,
                            const SortedIndex &sortedIdx);

#endif // RESOURCE_JOURNEY_QUERY_H