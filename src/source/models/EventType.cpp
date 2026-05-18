/**
 * @file        EventType.cpp
 * @brief       Implementation của các hàm chuyển đổi cho EventType.
 */

#include "models/EventType.h"

// ================================================================================
//  Helper functions
// ================================================================================

/**
 * @brief So sánh hai chuỗi không phân biệt hoa thường.
 * @note Hàm nội bộ, chỉ dùng trong file này.
 */
static bool streq(const std::string &a, const std::string &b)
{
    if (a.length() != b.length())
        return false;

    for (size_t i = 0; i < a.length(); ++i)
    {
        char ca = (a[i] >= 'a' && a[i] <= 'z') ? a[i] - ('a' - 'A') : a[i];
        char cb = (b[i] >= 'a' && b[i] <= 'z') ? b[i] - ('a' - 'A') : b[i];
        if (ca != cb)
            return false;
    }
    return true;
}

// ================================================================================
//  Public functions
// ================================================================================

EventType parseEventType(const std::string &s)
{
    if (streq(s, "LOGIN"))
        return LOGIN;
    if (streq(s, "LOGOUT"))
        return LOGOUT;
    if (streq(s, "TOKEN_REFRESH"))
        return TOKEN_REFRESH;
    if (streq(s, "ACCESS"))
        return ACCESS;
    if (streq(s, "FAILED_LOGIN"))
        return FAILED_LOGIN;
    if (streq(s, "OPEN_APP"))
        return OPEN_APP;
    if (streq(s, "DOWNLOAD"))
        return DOWNLOAD;
    if (streq(s, "ADMIN_ACTION"))
        return ADMIN_ACTION;

    return UNKNOWN_EVENT;
}

std::string eventTypeToString(EventType type)
{
    switch (type)
    {
    case LOGIN:
        return "LOGIN";
    case LOGOUT:
        return "LOGOUT";
    case TOKEN_REFRESH:
        return "TOKEN_REFRESH";
    case ACCESS:
        return "ACCESS";
    case FAILED_LOGIN:
        return "FAILED_LOGIN";
    case OPEN_APP:
        return "OPEN_APP";
    case DOWNLOAD:
        return "DOWNLOAD";
    case ADMIN_ACTION:
        return "ADMIN_ACTION";
    case UNKNOWN_EVENT:
        return "UNKNOWN_EVENT";
    default:
        return "UNKNOWN_EVENT";
    }
}