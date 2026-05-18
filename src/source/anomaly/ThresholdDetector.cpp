/**
 * @file        ThresholdDetector.cpp
 * @brief       Implementation các hàm kiểm tra và phát hiện bất thường theo ngưỡng cố định.
 */

#include "anomaly/ThresholdDetector.h"
#include "anomaly/AnomalyConfig.h"
#include "utils/TimeUtils.h"
#include "utils/StringUtils.h"

// ================================================================================
//  Helper functions
// ================================================================================

/**
 * @brief  Kiểm tra xem một chuỗi ký tự đã tồn tại trong mảng tĩnh hay chưa.
 * @note   Hàm nội bộ bổ trợ cho việc đếm các giá trị định danh duy nhất (Unique ID).
 * @param  arr Mảng các chuỗi cần kiểm tra
 * @param  count Số lượng phần tử hiện tại trong mảng
 * @param  val Giá trị chuỗi cần tìm kiếm
 * @return true nếu tìm thấy phần tử trùng khớp, false nếu ngược lại
 */
static bool existsInArray(const std::string *arr, int count, const std::string &val)
{
    for (int i = 0; i < count; i++)
    {
        if (strEquals(arr[i], val))
            return true;
    }
    return false;
}

// ================================================================================
//  Public functions
// ================================================================================

void detectMultiDeviceLogin(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                // Mảng lưu trữ danh sách thiết bị duy nhất trong cửa sổ trượt 60 phút
                std::string uniqueDevices[AnomalyConfig::MAX_DEVICES_PER_HOUR + 1];
                int uniqueCount = 0;

                for (int j = i; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 3600)
                    {
                        break;
                    }

                    if (!existsInArray(uniqueDevices, uniqueCount, pastEvent->device_id))
                    {
                        if (uniqueCount < AnomalyConfig::MAX_DEVICES_PER_HOUR + 1)
                        {
                            uniqueDevices[uniqueCount++] = pastEvent->device_id;
                        }
                    }
                }

                if (uniqueCount > AnomalyConfig::MAX_DEVICES_PER_HOUR)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::MULTI_DEVICE_LOGIN;
                    rec->userId = node->key;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "User used > " + std::to_string(AnomalyConfig::MAX_DEVICES_PER_HOUR) + " devices in 60 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}

void detectConsecutiveFailedLogin(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            int failedStreak = 0;
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *event = node->values[i];
                if (event->event_type == EventType::FAILED_LOGIN)
                {
                    failedStreak++;
                    if (failedStreak > AnomalyConfig::MAX_FAILED_LOGIN_STREAK)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::CONSECUTIVE_FAILED_LOGIN;
                        rec->userId = node->key;
                        rec->timestamp = event->timestamp;
                        rec->detail = "Consecutive failed logins > " + std::to_string(AnomalyConfig::MAX_FAILED_LOGIN_STREAK);
                        pushAnomaly(results, rec);
                    }
                }
                else
                {
                    // Bất kỳ sự kiện nào khác (ví dụ: LOGIN thành công) đều làm ngắt chuỗi đăng nhập sai
                    failedStreak = 0;
                }
            }
            node = node->next;
        }
    }
}

void detectResourceFlood(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byDevice.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byDevice.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                // Mảng tĩnh tối ưu bộ nhớ thu thập danh sách tài nguyên trong 30 phút
                std::string uniqueResources[AnomalyConfig::MAX_RESOURCES_PER_30MIN + 1];
                int uniqueCount = 0;

                for (int j = i; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 1800) // 30 mins
                    {
                        break;
                    }

                    if (!existsInArray(uniqueResources, uniqueCount, pastEvent->resource_id))
                    {
                        if (uniqueCount < AnomalyConfig::MAX_RESOURCES_PER_30MIN + 1)
                        {
                            uniqueResources[uniqueCount++] = pastEvent->resource_id;
                        }
                    }
                }

                if (uniqueCount > AnomalyConfig::MAX_RESOURCES_PER_30MIN)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::RESOURCE_FLOOD;
                    rec->userId = currentEvent->user_id; // Đánh dấu ID người dùng điều khiển thiết bị gây flood
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "Device " + node->key + " accessed > " + std::to_string(AnomalyConfig::MAX_RESOURCES_PER_30MIN) + " unique resources in 30 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}

void detectOffHoursAccess(AnomalyList &results, const DataStore &store)
{
    for (int i = 0; i < store.count; i++)
    {
        LogRecord *event = store.records[i];
        if (isOffHours(event->timestamp))
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::OFF_HOURS_ACCESS;
            rec->userId = event->user_id;
            rec->timestamp = event->timestamp;
            rec->detail = "Access during off-hours";
            pushAnomaly(results, rec);
        }
    }
}