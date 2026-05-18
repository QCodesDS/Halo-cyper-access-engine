/**
 * @file        QueryEngine.h
 * @brief       Định nghĩa bộ điều phối và phân tích cú pháp truy vấn (Query Engine) cho hệ thống log.
 *
 * Nhận chuỗi lệnh đầu vào từ giao diện dòng lệnh, phân tích các tham số (Khóa định danh, 
 * khoảng thời gian) và chuyển hướng xử lý đến các module truy vấn hành trình hoặc thống kê tương ứng.
 */

#ifndef QUERY_ENGINE_H
#define QUERY_ENGINE_H

#include <string>
#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"

/**
 * @brief  Hàm phân phối truy vấn chính - Phân tích chuỗi lệnh đầu vào và thực thi logic truy vấn tương ứng.
 * @note   Hỗ trợ các định dạng lệnh sau:
 *         - "query user <uid> <t_start> <t_end>": Truy vấn hành trình của người dùng.
 *         - "query resource <rid> <t_start> <t_end>": Truy vấn lịch sử truy cập tài nguyên.
 *         - "top resources <t_start> <t_end>": Thống kê tài nguyên được truy cập nhiều nhất.
 * @param  input Chuỗi lệnh thô nhận từ người dùng hoặc kịch bản kiểm thử
 * @param  hashIdx Hệ thống chỉ mục băm đa chiều phục vụ tìm kiếm theo khóa
 * @param  sortedIdx Chỉ mục mảng con trỏ đã sắp xếp phục vụ lọc khoảng thời gian
 */
void executeQuery(const std::string &input, const HashIndex &hashIdx, const SortedIndex &sortedIdx);

#endif // QUERY_ENGINE_H