/**
 * @file        CsvLoader.h
 * @brief       Module tải dữ liệu từ file CSV vào DataStore.
 *
 * Hỗ trợ đọc file CSV theo định dạng chuẩn của hệ thống log an ninh.
 */

#ifndef CSV_LOADER_H
#define CSV_LOADER_H

#include <string>

/// @brief Kết quả trả về sau khi load file CSV
struct LoadResult
{
    int loaded;  ///< Số bản ghi tải thành công
    int skipped; ///< Số bản ghi bị bỏ qua (lỗi hoặc không hợp lệ)
};

struct DataStore; // Forward declaration

/**
 * @brief Tải dữ liệu từ file CSV vào DataStore.
 *
 * @param filepath Đường dẫn đến file CSV
 * @param store DataStore đích (sẽ nhận các LogRecord được tạo)
 * @return LoadResult chứa thông tin số lượng loaded và skipped
 */
[[nodiscard]] LoadResult load(const std::string &filepath, DataStore &store);

#endif // CSV_LOADER_H