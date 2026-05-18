/**
 * @file        Validator.cpp
 * @brief       Implementation của các hàm kiểm tra tính hợp lệ dữ liệu.
 */

#include "storage/Validator.h"
#include <iostream>

// ================================================================================
//  Helper functions
// ================================================================================

bool isValidString(const std::string &s)
{
    return !s.empty(); // Modern & clear way
}

bool isValidTimestamp(long long ts)
{
    return ts > 0;
}

// ================================================================================
//  Main validation function
// ================================================================================

bool isValid(const LogRecord &r)
{
    // Kiểm tra các trường string
    if (!isValidString(r.user_id))
    {
        std::cerr << "[WARNING] Invalid user_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.device_id))
    {
        std::cerr << "[WARNING] Invalid device_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.app_id))
    {
        std::cerr << "[WARNING] Invalid app_id (empty)" << std::endl;
        return false;
    }

    if (!isValidString(r.resource_id))
    {
        std::cerr << "[WARNING] Invalid resource_id (empty)" << std::endl;
        return false;
    }

    // Kiểm tra enum
    if (r.event_type == UNKNOWN_EVENT)
    {
        std::cerr << "[WARNING] Invalid event_type (UNKNOWN_EVENT)" << std::endl;
        return false;
    }

    if (r.location == UNKNOWN_LOCATION)
    {
        std::cerr << "[WARNING] Invalid location (UNKNOWN_LOCATION)" << std::endl;
        return false;
    }

    // Kiểm tra timestamp
    if (!isValidTimestamp(r.timestamp))
    {
        std::cerr << "[WARNING] Invalid timestamp (" << r.timestamp << ")" << std::endl;
        return false;
    }

    return true;
}