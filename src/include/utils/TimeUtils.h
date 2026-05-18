/**
 * @file        TimeUtils.h
 * @brief       Các hàm tiện ích xử lý thời gian Unix epoch.
 */

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <string>

/**
 * @brief Chuyển Unix timestamp sang chuỗi thời gian dễ đọc (UTC).
 * @param epoch Thời gian Unix timestamp (giây)
 * @return Chuỗi định dạng "YYYY-MM-DD HH:MM:SS"
 */
std::string epochToReadable(long long epoch);

/**
 * @brief Kiểm tra xem thời gian có nằm ngoài giờ làm việc (08:00 - 18:00 UTC) hay không.
 * @param epoch Thời gian Unix timestamp (giây)
 * @return `true` nếu là ngoài giờ làm việc
 */
bool isOffHours(long long epoch);

/**
 * @brief Lấy giờ (0-23) từ Unix timestamp theo múi giờ UTC.
 * @param epoch Thời gian Unix timestamp (giây)
 * @return Giờ trong ngày (0-23)
 */
int getHourUTC(long long epoch);

#endif // TIME_UTILS_H