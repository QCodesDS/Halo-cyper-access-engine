/**
 * @file        IndexManager.h
 * @brief       Định nghĩa bộ quản lý chỉ mục tích hợp (Index Manager) cho hệ
 * thống log.
 *
 * Điều phối đồng thời chỉ mục băm đa chiều (HashIndex) và chỉ mục sắp xếp theo
 * thời gian (SortedIndex), cung cấp cơ chế tự động tính toán cấu hình tối ưu
 * dựa trên quy mô dữ liệu.
 */

#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H

#include "indexing/HashIndex.h"
#include "indexing/SortedIndex.h"
#include "storage/DataStore.h"

/**
 * @struct IndexManager
 * @brief  Cấu trúc quản lý tập hợp toàn bộ các loại chỉ mục trong hệ thống.
 */
struct IndexManager {
  HashIndex hashIdx; ///< Hệ thống chỉ mục băm đa chiều (User, Device, Resource)
  SortedIndex
      sortedIdx; ///< Chỉ mục mảng con trỏ sắp xếp tăng dần theo mốc thời gian
  bool isBuilt; ///< Cờ đánh dấu trạng thái các chỉ mục đã được xây dựng thành
                ///< công hay chưa
};

/**
 * @brief Khởi tạo và xây dựng toàn bộ hệ thống chỉ mục từ kho lưu trữ
 * DataStore.
 * @note  Hàm tự động lựa chọn kích thước bảng băm (Table Size) tối ưu là số
 * nguyên tố dựa trên số lượng bản ghi. Đồng thời in thông tin thời gian khởi
 * tạo ra console theo định dạng: "[INFO] Indexes built in Xs"
 * @param mgr Tham chiếu tới bộ quản lý chỉ mục cần nạp dữ liệu
 * @param store Kho dữ liệu log hệ thống nguồn
 */
void buildAllIndexes(IndexManager &mgr, DataStore &store);

/**
 * @brief Xóa bỏ dữ liệu và giải phóng toàn bộ bộ nhớ của tất cả các chỉ mục
 * thành phần.
 * @param mgr Tham chiếu tới bộ quản lý chỉ mục cần giải phóng
 */
void clearAllIndexes(IndexManager &mgr);

#endif // INDEX_MANAGER_H