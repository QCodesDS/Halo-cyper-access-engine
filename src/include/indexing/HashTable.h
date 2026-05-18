/**
 * @file        HashTable.h
 * @brief       Định nghĩa cấu trúc dữ liệu bảng băm (Hash Table) xử lý va chạm
 * bằng Separate Chaining.
 *
 * Cung cấp giải pháp lưu trữ các con trỏ bản ghi log theo cơ chế băm chuỗi nội
 * bộ, hỗ trợ cấp phát động mảng giá trị cho các khóa trùng lặp.
 */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "models/LogRecord.h"
#include <string>


/**
 * @struct HashNode
 * @brief  Nút trong danh sách liên kết đơn phục vụ kỹ thuật giải quyết va chạm
 * băm (Separate Chaining).
 */
struct HashNode {
  std::string key; ///< Khóa băm (Dạng chuỗi ký tự định danh)
  LogRecord *
      *values;    ///< Mảng động chứa các con trỏ quản lý LogRecord trùng khóa
  int count;      ///< Số lượng phần tử hiện tại có trong mảng values
  int capacity;   ///< Sức chứa tối đa hiện tại của mảng values
  HashNode *next; ///< Con trỏ trỏ đến nút kế tiếp trong chuỗi va chạm
};

/**
 * @struct HashTable
 * @brief  Cấu trúc thực thể bảng băm quản lý danh sách các bucket.
 */
struct HashTable {
  HashNode **buckets; ///< Mảng các con trỏ bucket (mỗi bucket quản lý một danh
                      ///< sách liên kết)
  int tableSize; ///< Tổng số lượng bucket của bảng băm
  int totalKeys; ///< Tổng số lượng khóa duy nhất (Unique keys) hiện có trong
                 ///< bảng băm
};

/**
 * @brief Khởi tạo bảng băm với kích thước xác định ban đầu.
 * @param ht Tham chiếu tới cấu trúc bảng băm cần khởi tạo
 * @param tableSize Số lượng ô lưu trữ (bucket) cấp phát cho bảng băm
 */
void initHashTable(HashTable &ht, int tableSize);

/**
 * @brief Thêm một con trỏ bản ghi LogRecord vào bảng băm theo khóa chỉ định.
 * @param ht Tham chiếu tới bảng băm đích
 * @param key Chuỗi ký tự định danh dùng làm khóa phân loại
 * @param record Con trỏ vùng nhớ chứa bản ghi log hệ thống
 */
void insertHash(HashTable &ht, const std::string &key, LogRecord *record);

/**
 * @brief Tìm kiếm và lấy ra danh sách toàn bộ bản ghi log tương ứng với khóa.
 * @param ht Tham chiếu tới bảng băm cần truy vấn
 * @param key Khóa định danh chuỗi cần tìm
 * @param outCount Tham chiếu nhận số lượng phần tử tìm thấy trong mảng kết quả
 * @return Mảng các con trỏ trỏ đến các LogRecord hợp lệ, hoặc trả về nullptr
 * nếu khóa không tồn tại
 */
LogRecord **lookupHash(const HashTable &ht, const std::string &key,
                       int &outCount);

/**
 * @brief Giải phóng toàn bộ các nút liên kết và các mảng động lưu trữ giá trị
 * trong bảng băm.
 * @param ht Tham chiếu tới bảng băm cần giải phóng bộ nhớ
 */
void clearHashTable(HashTable &ht);

/**
 * @brief Hàm băm cuốn đa thức (Polynomial Rolling Hash Function) chuyển đổi
 * chuỗi thành chỉ mục số.
 * @param key Chuỗi dữ liệu đầu vào cần băm
 * @param tableSize Giới hạn trên của chỉ mục trả về (Kích thước bảng băm)
 * @return Giá trị băm nguyên nằm trong khoảng [0, tableSize - 1]
 */
int hashFunction(const std::string &key, int tableSize);

#endif // HASH_TABLE_H