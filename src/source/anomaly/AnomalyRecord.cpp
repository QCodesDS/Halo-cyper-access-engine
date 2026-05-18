/**
 * @file        AnomalyRecord.cpp
 * @brief       Implementation các hàm quản lý bộ nhớ và danh sách mảng động AnomalyList.
 */

#include "anomaly/AnomalyRecord.h"

// ================================================================================
//  Public functions
// ================================================================================

void initAnomalyList(AnomalyList &list, int capacity)
{
    list.capacity = capacity;
    list.count = 0;
    list.records = new AnomalyRecord *[capacity];
}

void pushAnomaly(AnomalyList &list, AnomalyRecord *record)
{
    if (list.count == list.capacity)
    {
        int newCapacity = list.capacity * 2;
        if (newCapacity == 0) newCapacity = 100;
        
        AnomalyRecord **newRecords = new AnomalyRecord *[newCapacity];
        for (int i = 0; i < list.count; i++)
        {
            newRecords[i] = list.records[i];
        }
        
        delete[] list.records;
        list.records = newRecords;
        list.capacity = newCapacity;
    }
    list.records[list.count++] = record;
}

void clearAnomalyList(AnomalyList &list)
{
    if (list.records != nullptr)
    {
        for (int i = 0; i < list.count; i++)
        {
            delete list.records[i];
        }
        delete[] list.records;
        list.records = nullptr;
    }
    list.count = 0;
    list.capacity = 0;
}