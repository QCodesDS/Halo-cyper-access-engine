/**
 * @file        Validator.h
 * @brief       Các hàm kiểm tra tính hợp lệ của LogRecord theo yêu cầu nghiệp vụ.
 *
 * Cung cấp các hàm validate cơ bản cho dữ liệu log an ninh hệ thống.
 */

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "models/LogRecord.h"

/**
 * @brief Kiểm tra toàn bộ tính hợp lệ của một LogRecord.
 *
 * Kiểm tra tất cả các trường bắt buộc theo BRD:
 * - Các trường string không được rỗng
 * - event_type và location không được UNKNOWN
 * - timestamp phải hợp lệ
 *
 * @param r Bản ghi cần kiểm tra
 * @return `true` nếu bản ghi hợp lệ, ngược lại `false`
 */
[[nodiscard]] bool isValid(const LogRecord &r);

/**
 * @brief Kiểm tra một chuỗi có rỗng hay không.
 * @param s Chuỗi cần kiểm tra
 * @return `true` nếu chuỗi có độ dài > 0
 */
[[nodiscard]] bool isValidString(const std::string &s);

/**
 * @brief Kiểm tra timestamp có hợp lệ hay không.
 * @param ts Unix timestamp cần kiểm tra
 * @return `true` nếu timestamp > 0
 */
[[nodiscard]] bool isValidTimestamp(long long ts);

#endif // VALIDATOR_H