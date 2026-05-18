/**
 * @file        Deduplicator.h
 * @brief       Module loại bỏ các bản ghi trùng lặp trong DataStore.
 */

#ifndef DEDUPLICATOR_H
#define DEDUPLICATOR_H

#include "models/LogRecord.h"

struct DataStore;

/**
 * @brief Loại bỏ các bản ghi LogRecord trùng lặp trong DataStore.
 *
 * Hai bản ghi được coi là trùng nếu tất cả 7 trường đều giống nhau.
 * Hàm giữ lại bản ghi đầu tiên xuất hiện, xóa các bản ghi trùng sau.
 *
 * @param store DataStore cần xử lý (sẽ bị thay đổi trực tiếp)
 * @return Số lượng bản ghi bị loại bỏ
 */
[[nodiscard]] int deduplicate(DataStore &store);

/**
 * @brief So sánh hai LogRecord có giống hệt nhau không.
 * @param r1 Bản ghi thứ nhất
 * @param r2 Bản ghi thứ hai
 * @return `true` nếu tất cả các trường đều bằng nhau
 */
[[nodiscard]] bool recordsEqual(const LogRecord &r1, const LogRecord &r2);

#endif // DEDUPLICATOR_H