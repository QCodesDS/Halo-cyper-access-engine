/**
 * @file        AdvancedDetector.cpp
 * @brief       Implementation của các thuật toán phát hiện hành vi bất thường nâng cao.
 */

#include "anomaly/AdvancedDetector.h"
#include "anomaly/AnomalyConfig.h"

// ================================================================================
//  Public functions
// ================================================================================

void detectBruteForce(AnomalyList &results, const HashIndex &hashIdx)
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
                }
                else if (event->event_type == EventType::LOGIN)
                {
                    if (failedStreak >= AnomalyConfig::MAX_FAILED_LOGIN_STREAK)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::BRUTE_FORCE;
                        rec->userId = node->key;
                        rec->timestamp = event->timestamp;
                        rec->detail = std::to_string(failedStreak) + " fails -> success [BONUS]";
                        pushAnomaly(results, rec);
                    }
                    failedStreak = 0; // Reset sau khi LOGIN
                }
                else
                {
                    // Các sự kiện khác cũng làm đứt chuỗi brute force
                    failedStreak = 0;
                }
            }
            node = node->next;
        }
    }
}

void detectDormantActivation(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            if (node->count < 2) 
            {
                node = node->next;
                continue;
            }

            for (int i = 1; i < node->count; i++)
            {
                LogRecord *prevEvent = node->values[i - 1];
                LogRecord *currentEvent = node->values[i];

                long long gap = currentEvent->timestamp - prevEvent->timestamp;
                if (gap > (AnomalyConfig::DORMANT_DAYS * 86400)) // 7 days
                {
                    // Đã ngủ đông > 7 ngày. Giờ đếm số event trong 1 giờ tới.
                    int burstCount = 0;
                    for (int j = i; j < node->count; j++)
                    {
                        if (node->values[j]->timestamp - currentEvent->timestamp > 3600)
                        {
                            break;
                        }
                        burstCount++;
                    }

                    if (burstCount > AnomalyConfig::BURST_EVENTS_PER_HOUR)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::DORMANT_ACTIVATION;
                        rec->userId = node->key;
                        rec->timestamp = currentEvent->timestamp;
                        rec->detail = "Dormant for " + std::to_string(gap / 86400) + " days, then " + std::to_string(burstCount) + " events in 1h [BONUS]";
                        pushAnomaly(results, rec);
                    }
                }
            }
            node = node->next;
        }
    }
}