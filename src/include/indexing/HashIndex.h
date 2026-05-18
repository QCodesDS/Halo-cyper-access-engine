/**
 * @file        HashIndex.h
 * @brief       Định nghĩa cấu trúc chỉ mục băm đa chiều (Hash Index) cho hệ
 * thống log.
 *
 * Cung cấp giải pháp lập chỉ mục đồng thời trên 3 trường thông tin cốt lõi:
 * user_id, device_id và resource_id nhằm tối ưu hóa tốc độ truy vấn đa chiều
 * cho các bộ dò tìm.
 */

#ifndef HASH_INDEX_H
#define HASH_INDEX_H

#include "indexing/HashTable.h"
#include "storage/DataStore.h"

/**
 * @struct HashIndex
 * @brief  Cấu trúc quản lý tập hợp các bảng băm phục vụ lập chỉ mục đa chiều.
 */
struct HashIndex {
  HashTable byUser;   ///< Bảng băm lập chỉ mục theo mã người dùng (user_id)
  HashTable byDevice; ///< Bảng băm lập chỉ mục theo mã thiết bị (device_id)
  HashTable
      byResource; ///< Bảng băm lập chỉ mục theo mã tài nguyên (resource_id)
  int tableSize; ///< Kích thước số lượng bucket áp dụng chung cho cả 3 bảng băm
};

/**
 * @brief Xây dựng các chỉ mục băm đồng bộ từ kho dữ liệu thô DataStore.
 * @param idx Tham chiếu tới cấu trúc HashIndex cần khởi tạo và nạp dữ liệu
 * @param store Kho lưu trữ bản ghi log nguồn
 * @param tableSize Kích thước mảng bucket mong muốn cho các bảng băm nội bộ
 */
void buildHashIndex(HashIndex &idx, DataStore &store, int tableSize);

/**
 * @brief Truy vấn danh sách các bản ghi log dựa trên mã định danh người dùng
 * (User ID).
 * @param idx Tham chiếu tới cấu trúc chỉ mục băm đa chiều
 * @param userId Mã định danh người dùng cần tìm kiếm
 * @param outCount Tham chiếu nhận số lượng bản ghi log tìm thấy
 * @return Mảng các con trỏ trỏ đến LogRecord kết quả, trả về nullptr nếu không
 * tìm thấy
 */
LogRecord **getByUser(const HashIndex &idx, const std::string &userId,
                      int &outCount);

/**
 * @brief Truy vấn danh sách các bản ghi log dựa trên mã định danh thiết bị
 * (Device ID).
 * @param idx Tham chiếu tới cấu trúc chỉ mục băm đa chiều
 * @param deviceId Mã định danh thiết bị cần tìm kiếm
 * @param outCount Tham chiếu nhận số lượng bản ghi log tìm thấy
 * @return Mảng các con trỏ trỏ đến LogRecord kết quả, trả về nullptr nếu không
 * tìm thấy
 */
LogRecord **getByDevice(const HashIndex &idx, const std::string &deviceId,
                        int &outCount);

/**
 * @brief Truy vấn danh sách các bản ghi log dựa trên mã định danh tài nguyên
 * (Resource ID).
 * @param idx Tham chiếu tới cấu trúc chỉ mục băm đa chiều
 * @param resourceId Mã định danh tài nguyên cần tìm kiếm
 * @param outCount Tham chiếu nhận số lượng bản ghi log tìm thấy
 * @return Mảng các con trỏ trỏ đến LogRecord kết quả, trả về nullptr nếu không
 * tìm thấy
 */
LogRecord **getByResource(const HashIndex &idx, const std::string &resourceId,
                          int &outCount);

/**
 * @brief Xóa bỏ và giải phóng toàn bộ tài nguyên, bộ nhớ của cấu trúc chỉ mục
 * băm đa chiều.
 * @param idx Tham chiếu tới cấu trúc chỉ mục băm cần thu hồi
 */
void clearHashIndex(HashIndex &idx);

#endif // HASH_INDEX_H