/**
 * @file        StringUtils.cpp
 * @brief       Implementation của các hàm tiện ích chuỗi.
 */

#include "utils/StringUtils.h"

// ================================================================================
//  Public functions
// ================================================================================

std::string *split(const std::string &s, char delim, int &outCount)
{
    if (s.empty())
    {
        outCount = 0;
        return nullptr;
    }

    // First pass: đếm số token
    int count = 1;
    for (char c : s)
    {
        if (c == delim)
            ++count;
    }

    outCount = count;
    std::string *tokens = new std::string[count];

    // Second pass: tách chuỗi
    int tokenIndex = 0;
    size_t start = 0;

    for (size_t i = 0; i <= s.length(); ++i)
    {
        if (i == s.length() || s[i] == delim)
        {
            tokens[tokenIndex] = s.substr(start, i - start);
            ++tokenIndex;
            start = i + 1;
        }
    }

    return tokens;
}

std::string trim(const std::string &s)
{
    size_t start = 0;
    size_t end = s.length();

    // Bỏ khoảng trắng đầu
    while (start < end && (s[start] == ' ' || s[start] == '\t'))
        ++start;

    // Bỏ khoảng trắng cuối
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t'))
        --end;

    return s.substr(start, end - start);
}

bool strEquals(const std::string &a, const std::string &b)
{
    if (a.length() != b.length())
        return false;

    for (size_t i = 0; i < a.length(); ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}