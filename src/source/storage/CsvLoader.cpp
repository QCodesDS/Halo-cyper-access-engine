/**
 * @file        CsvLoader.cpp
 * @brief       Implementation của module tải dữ liệu CSV.
 */

#include "storage/CsvLoader.h"
#include "storage/DataStore.h"
#include "storage/Validator.h"
#include "utils/StringUtils.h"
#include "models/EventType.h"
#include "models/Location.h"
#include <fstream>
#include <iostream>
#include <string>

LoadResult load(const std::string &filepath, DataStore &store)
{
    LoadResult result{0, 0};

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] Cannot open file: " << filepath << std::endl;
        return result;
    }

    std::string line;
    int lineNum = 0;

    // Skip header line
    if (!std::getline(file, line))
    {
        std::cerr << "[ERROR] Empty file: " << filepath << std::endl;
        return result;
    }
    ++lineNum;

    // Read data lines
    while (std::getline(file, line))
    {
        ++lineNum;

        line = trim(line);
        if (line.empty())
        {
            ++result.skipped;
            continue;
        }

        // Tách các trường bằng dấu phẩy
        int tokenCount = 0;
        std::string *tokens = split(line, ',', tokenCount);

        if (tokenCount != 7)
        {
            std::cerr << "[WARNING] Line " << lineNum
                      << ": Expected 7 fields, got " << tokenCount << std::endl;
            ++result.skipped;
            delete[] tokens;
            continue;
        }

        // Trim tất cả các trường
        std::string user_id = trim(tokens[0]);
        std::string device_id = trim(tokens[1]);
        std::string app_id = trim(tokens[2]);
        std::string resource_id = trim(tokens[3]);
        std::string event_str = trim(tokens[4]);
        std::string location_str = trim(tokens[5]);
        std::string timestamp_str = trim(tokens[6]);

        // Parse timestamp
        long long timestamp = 0;
        try
        {
            timestamp = std::stoll(timestamp_str);
        }
        catch (...)
        {
            std::cerr << "[WARNING] Line " << lineNum
                      << ": Invalid timestamp: " << timestamp_str << std::endl;
            ++result.skipped;
            delete[] tokens;
            continue;
        }

        // Tạo LogRecord
        LogRecord *record = new LogRecord(
            user_id,
            device_id,
            app_id,
            resource_id,
            parseEventType(event_str),
            parseLocation(location_str),
            timestamp);

        // Kiểm tra tính hợp lệ
        if (!isValid(*record))
        {
            ++result.skipped;
            delete record;
            delete[] tokens;
            continue;
        }

        // Thêm vào DataStore
        push(store, record);
        ++result.loaded;

        delete[] tokens;
    }

    file.close();
    return result;
}