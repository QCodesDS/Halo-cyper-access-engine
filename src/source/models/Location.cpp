/**
 * @file        Location.cpp
 * @brief       Implementation của các hàm chuyển đổi cho Location.
 */

#include "models/Location.h"

// ================================================================================
//  Helper functions
// ================================================================================

/**
 * @brief So sánh hai chuỗi không phân biệt hoa thường.
 * @note Hàm static nội bộ, chỉ sử dụng trong file này.
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

Location parseLocation(const std::string &s)
{
    if (streq(s, "US"))
        return US;
    if (streq(s, "VN"))
        return VN;
    if (streq(s, "JP"))
        return JP;
    if (streq(s, "KR"))
        return KR;
    if (streq(s, "SG"))
        return SG;
    if (streq(s, "CN"))
        return CN;
    if (streq(s, "DE"))
        return DE;
    if (streq(s, "FR"))
        return FR;
    if (streq(s, "UK"))
        return UK;
    if (streq(s, "AU"))
        return AU;
    if (streq(s, "CA"))
        return CA;
    if (streq(s, "IN"))
        return IN;
    if (streq(s, "BR"))
        return BR;
    if (streq(s, "RU"))
        return RU;
    if (streq(s, "TH"))
        return TH;

    return UNKNOWN_LOCATION;
}

std::string locationToString(Location loc)
{
    switch (loc)
    {
    case US:
        return "US";
    case VN:
        return "VN";
    case JP:
        return "JP";
    case KR:
        return "KR";
    case SG:
        return "SG";
    case CN:
        return "CN";
    case DE:
        return "DE";
    case FR:
        return "FR";
    case UK:
        return "UK";
    case AU:
        return "AU";
    case CA:
        return "CA";
    case IN:
        return "IN";
    case BR:
        return "BR";
    case RU:
        return "RU";
    case TH:
        return "TH";
    case UNKNOWN_LOCATION:
        return "UNKNOWN_LOCATION";
    default:
        return "UNKNOWN_LOCATION";
    }
}