/**
 * @file        HashIndex.cpp
 * @brief       Implementation các hàm phân phối và truy vấn đa chiều thông qua HashIndex.
 */

#include "indexing/HashIndex.h"

// ================================================================================
//  Public functions
// ================================================================================

void buildHashIndex(HashIndex &idx, DataStore &store, int tableSize)
{
    idx.tableSize = tableSize;

    // Khởi tạo đồng bộ cả 3 bảng băm thành phần
    initHashTable(idx.byUser, tableSize);
    initHashTable(idx.byDevice, tableSize);
    initHashTable(idx.byResource, tableSize);

    // Duyệt qua toàn bộ bản ghi trong DataStore để phân loại vào các chiều chỉ mục
    for (int i = 0; i < store.count; i++)
    {
        LogRecord *record = get(store, i);

        if (record != nullptr)
        {
            insertHash(idx.byUser, record->user_id, record);
            insertHash(idx.byDevice, record->device_id, record);
            insertHash(idx.byResource, record->resource_id, record);
        }
    }
}

LogRecord **getByUser(const HashIndex &idx, const std::string &userId, int &outCount)
{
    return lookupHash(idx.byUser, userId, outCount);
}

LogRecord **getByDevice(const HashIndex &idx, const std::string &deviceId, int &outCount)
{
    return lookupHash(idx.byDevice, deviceId, outCount);
}

LogRecord **getByResource(const HashIndex &idx, const std::string &resourceId, int &outCount)
{
    return lookupHash(idx.byResource, resourceId, outCount);
}

void clearHashIndex(HashIndex &idx)
{
    clearHashTable(idx.byUser);
    clearHashTable(idx.byDevice);
    clearHashTable(idx.byResource);
    idx.tableSize = 0;
}