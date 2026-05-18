/**
 * @file        TopResourceQuery.h
 * @brief       Định nghĩa cấu trúc và hàm xử lý truy vấn thống kê tài nguyên hệ
 * thống phổ biến (Top Resources).
 *
 * Cung cấp giải pháp đếm tần suất xuất hiện của các tài nguyên trong một khoảng
 * thời gian xác định và xếp hạng chúng theo thứ tự giảm dần để nhận diện xu
 * hướng truy cập.
 */

#ifndef TOP_RESOURCE_QUERY_H
#define TOP_RESOURCE_QUERY_H

#include "indexing/SortedIndex.h"

/**
 * @struct CountEntry
 * @brief  Cấu trúc lưu trữ cặp thông tin khóa-giá trị phục vụ việc đếm số lần
 * xuất hiện của tài nguyên.
 */
struct CountEntry {
  std::string resourceId; ///< Mã định danh duy nhất của tài nguyên
  int count; ///< Số lần tài nguyên được truy cập trong khoảng thời gian xét
};

/**
 * @brief  Thực thi truy vấn và in ra danh sách N tài nguyên được truy cập nhiều
 * nhất.
 * @note   Sử dụng tìm kiếm nhị phân trên `SortedIndex` để khoanh vùng dữ liệu
 * theo thời gian, sau đó gom cụm đếm tần suất và sắp xếp bằng thuật toán
 * Selection Sort cục bộ.
 * @param  timeStart Mốc thời gian bắt đầu khoảng thống kê (Unix Timestamp)
 * @param  timeEnd Mốc thời gian kết thúc khoảng thống kê (Unix Timestamp)
 * @param  sortedIdx Chỉ mục mảng con trỏ đã sắp xếp theo thời gian
 * @param  topN Số lượng tài nguyên tối đa muốn hiển thị trong bảng xếp hạng
 * (Mặc định là 10)
 */
void executeTopResources(long long timeStart, long long timeEnd,
                         const SortedIndex &sortedIdx, int topN = 10);

#endif // TOP_RESOURCE_QUERY_H