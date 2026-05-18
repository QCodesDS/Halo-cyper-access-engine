/**
 * @file        EventType.h
 * @brief       Định nghĩa các loại sự kiện an ninh hệ thống.
 *
 * Enum này được sử dụng xuyên suốt hệ thống để ghi nhận và phân loại
 * các hoạt động liên quan đến bảo mật và truy cập tài nguyên.
 */

#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include <string>

/**
 * @enum EventType
 * @brief Các loại sự kiện an ninh hệ thống.
 */
enum EventType
{
    LOGIN,         ///< Đăng nhập thành công
    LOGOUT,        ///< Đăng xuất
    TOKEN_REFRESH, ///< Làm mới token
    ACCESS,        ///< Truy cập tài nguyên
    FAILED_LOGIN,  ///< Đăng nhập thất bại
    OPEN_APP,      ///< Mở ứng dụng
    DOWNLOAD,      ///< Tải xuống tài nguyên
    ADMIN_ACTION,  ///< Hành động của quản trị viên
    UNKNOWN_EVENT  ///< Loại sự kiện không xác định
};

/**
 * @brief Chuyển chuỗi thành EventType (không phân biệt hoa thường).
 * @param s Chuỗi đại diện cho loại sự kiện
 * @return EventType tương ứng, UNKNOWN_EVENT nếu không khớp
 */
EventType parseEventType(const std::string &s);

/**
 * @brief Chuyển EventType thành chuỗi string.
 * @param type Loại sự kiện
 * @return Tên của EventType dưới dạng string
 */
std::string eventTypeToString(EventType type);

#endif // EVENT_TYPE_H