/**
 * @file        StringUtils.h
 * @brief       Các hàm tiện ích xử lý chuỗi string.
 *
 * Cung cấp các chức năng phổ biến: split, trim, so sánh chuỗi.
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

/**
 * @brief Tách chuỗi theo ký tự phân cách.
 *
 * @note Hàm cấp phát động bằng `new[]`. Người gọi **phải** gọi `delete[]`
 *       để giải phóng bộ nhớ sau khi sử dụng.
 *
 * @param s Chuỗi đầu vào
 * @param delim Ký tự phân cách
 * @param outCount Số lượng token sau khi tách (được gán giá trị)
 * @return Mảng con trỏ chứa các token. Trả về `nullptr` nếu chuỗi rỗng.
 */
std::string *split(const std::string &s, char delim, int &outCount);

/**
 * @brief Loại bỏ khoảng trắng (space và tab) ở đầu và cuối chuỗi.
 * @param s Chuỗi đầu vào
 * @return Chuỗi đã được trim
 */
std::string trim(const std::string &s);

/**
 * @brief So sánh hai chuỗi có phân biệt hoa thường.
 * @param a Chuỗi thứ nhất
 * @param b Chuỗi thứ hai
 * @return `true` nếu hai chuỗi giống hệt nhau
 */
bool strEquals(const std::string &a, const std::string &b);

#endif // STRING_UTILS_H