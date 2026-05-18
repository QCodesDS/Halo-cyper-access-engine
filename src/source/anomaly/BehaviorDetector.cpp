/**
 * @file        BehaviorDetector.cpp
 * @brief       Implementation các thuật toán phân tích và phát hiện bất thường vị trí.
 */

#include "anomaly/BehaviorDetector.h"
#include "anomaly/AnomalyConfig.h"
#include "models/Location.h"

// ================================================================================
//  Public functions
// ================================================================================

void detectImpossibleTravel(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                if (currentEvent->location == Location::UNKNOWN_LOCATION) continue;

                // Quét ngược lại các sự kiện quá khứ nằm trong khung thời gian IMPOSSIBLE_TRAVEL_MINS
                for (int j = i - 1; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (pastEvent->location == Location::UNKNOWN_LOCATION) continue;
                    
                    if (currentEvent->timestamp - pastEvent->timestamp > (AnomalyConfig::IMPOSSIBLE_TRAVEL_MINS * 60))
                    {
                        break;
                    }

                    if (currentEvent->location != pastEvent->location)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::IMPOSSIBLE_TRAVEL;
                        rec->userId = node->key;
                        rec->timestamp = currentEvent->timestamp;
                        rec->detail = locationToString(pastEvent->location) + " -> " + locationToString(currentEvent->location) + " in " + std::to_string((currentEvent->timestamp - pastEvent->timestamp) / 60) + " min";
                        pushAnomaly(results, rec);
                        break; // Chỉ đánh dấu cảnh báo một lần cho sự kiện hiện tại này
                    }
                }
            }
            node = node->next;
        }
    }
}

void detectLocationChurning(AnomalyList &results, const HashIndex &hashIdx)
{
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr)
        {
            for (int i = 0; i < node->count; i++)
            {
                LogRecord *currentEvent = node->values[i];
                
                int locationChanges = 0;
                Location lastLoc = currentEvent->location;

                // Kiểm tra ngược dòng thời gian 60 phút để đếm số lần đổi vị trí liên tiếp
                for (int j = i - 1; j >= 0; j--)
                {
                    LogRecord *pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 3600)
                    {
                        break;
                    }
                    
                    if (pastEvent->location != Location::UNKNOWN_LOCATION && 
                        pastEvent->location != lastLoc)
                    {
                        locationChanges++;
                        lastLoc = pastEvent->location;
                    }
                }

                if (locationChanges > AnomalyConfig::MAX_LOCATION_CHANGES)
                {
                    AnomalyRecord *rec = new AnomalyRecord();
                    rec->type = AnomalyType::LOCATION_CHURNING;
                    rec->userId = node->key;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "Location changed " + std::to_string(locationChanges) + " times in 60 min";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}