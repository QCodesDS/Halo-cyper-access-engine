/**
 * @file        DataStore.h
 * @brief       Lưu trữ động danh sách các LogRecord.
 *
 * Cấu trúc này quản lý mảng động các bản ghi log, tự động mở rộng
 * khi cần (doubling capacity). DataStore sở hữu và chịu trách nhiệm
 * giải phóng bộ nhớ của các LogRecord bên trong.
 */

#ifndef DATASTORE_H
#define DATASTORE_H

#include "models/LogRecord.h"

/**
 * @struct DataStore
 * @brief  Container lưu trữ động các bản ghi log.
 *
 * @note   DataStore sở hữu (owns) các LogRecord* được push vào.
 *         Người dùng không được delete thủ công các record.
 */
struct DataStore
{
    LogRecord **records; ///< Mảng con trỏ đến LogRecord
    int capacity;        ///< Dung lượng hiện tại của mảng
    int count;           ///< Số lượng bản ghi thực tế

    /// Constructor mặc định (khởi tạo rỗng)
    DataStore();
};

/**
 * @brief Khởi tạo DataStore với dung lượng ban đầu.
 * @param store DataStore cần khởi tạo
 * @param initialCapacity Dung lượng khởi tạo (mặc định 1000)
 */
void initDataStore(DataStore &store, int initialCapacity = 1000);

/**
 * @brief Thêm một LogRecord vào kho lưu trữ.
 *
 * @note DataStore sẽ sở hữu con trỏ `r`. Không delete `r` từ bên ngoài.
 *
 * @param store DataStore đích
 * @param r Con trỏ đến LogRecord cần thêm
 */
void push(DataStore &store, LogRecord *r);

/**
 * @brief Lấy bản ghi tại vị trí index.
 * @param store DataStore
 * @param i Chỉ số
 * @return Con trỏ đến LogRecord, hoặc `nullptr` nếu index không hợp lệ
 */
[[nodiscard]] LogRecord *get(const DataStore &store, int i);

/**
 * @brief Trả về số lượng bản ghi hiện tại.
 * @param store DataStore
 * @return Số lượng LogRecord
 */
[[nodiscard]] int size(const DataStore &store);

/**
 * @brief Xóa toàn bộ dữ liệu và giải phóng bộ nhớ.
 * @param store DataStore cần dọn dẹp
 */
void clear(DataStore &store);

/**
 * @brief Trả về mảng con trỏ để duyệt hoặc sắp xếp.
 * @warning Không được thay đổi kích thước mảng qua con trỏ này.
 * @param store DataStore
 * @return Mảng con trỏ LogRecord*
 */
[[nodiscard]] LogRecord **getAll(const DataStore &store);

#endif // DATASTORE_H