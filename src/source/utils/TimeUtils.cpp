/**
 * @file        TimeUtils.cpp
 * @brief       Implementation của các hàm tiện ích thời gian.
 */

#include "utils/TimeUtils.h"
#include <ctime>
#include <cstdio>

std::string epochToReadable(long long epoch)
{
    time_t t = static_cast<time_t>(epoch);
    struct tm *utcTime = gmtime(&t);

    if (utcTime == nullptr)
        return "INVALID_TIME";

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             utcTime->tm_year + 1900,
             utcTime->tm_mon + 1,
             utcTime->tm_mday,
             utcTime->tm_hour,
             utcTime->tm_min,
             utcTime->tm_sec);

    return std::string(buffer);
}

int getHourUTC(long long epoch)
{
    time_t t = static_cast<time_t>(epoch);
    struct tm *utcTime = gmtime(&t);

    return (utcTime != nullptr) ? utcTime->tm_hour : -1;
}

bool isOffHours(long long epoch)
{
    int hour = getHourUTC(epoch);
    // Ngoài giờ làm việc: trước 08:00 hoặc từ 18:00 trở đi (theo UTC)
    return (hour < 8 || hour >= 18);
}