/**
 * @file        QueryEngine.cpp
 * @brief       Implementation cơ chế tách từ tố (tokenizing) và điều phối lệnh của bộ máy truy vấn.
 */

#include "query/QueryEngine.h"
#include "query/UserJourneyQuery.h"
#include "query/ResourceJourneyQuery.h"
#include "query/TopResourceQuery.h"
#include "utils/StringUtils.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ================================================================================
//  Public functions
// ================================================================================

void executeQuery(const std::string &input, const HashIndex &hashIdx, const SortedIndex &sortedIdx)
{
    // Loại bỏ khoảng trắng thừa ở hai đầu chuỗi lệnh
    std::string trimmed = trim(input);

    // Sao chép chuỗi sang mảng char kiểu C để sử dụng hàm strtok an toàn
    char *inputCopy = new char[trimmed.length() + 1];
    strcpy(inputCopy, trimmed.c_str());

    const char *delim = " ";
    char *token = strtok(inputCopy, delim);

    int tokenCount = 0;
    const char *tokens[10] = {nullptr};

    // Tiến hành phân rã chuỗi lệnh thành các từ tố (tokens) dựa trên khoảng trắng
    while (token != nullptr && tokenCount < 10)
    {
        tokens[tokenCount++] = token;
        token = strtok(nullptr, delim);
    }

    // Kiểm tra tính hợp lệ tối thiểu của số lượng từ tố nhận được
    if (tokenCount < 1)
    {
        printf("[ERROR] Invalid query format\n");
        delete[] inputCopy;
        return;
    }

    std::string cmd = tokens[0];

    // Phân vùng logic 1: Xử lý nhóm lệnh "query" (user hoặc resource)
    if (cmd == "query" && tokenCount >= 5)
    {
        std::string subCmd = tokens[1];

        if (subCmd == "user")
        {
            // Cú pháp: query user <uid> <t_start> <t_end>
            std::string userId = tokens[2];
            long long timeStart = atoll(tokens[3]);
            long long timeEnd = atoll(tokens[4]);

            UserJourneyQuery q;
            q.userId = userId;
            q.timeStart = timeStart;
            q.timeEnd = timeEnd;

            // Chuyển hướng thực thi sang module truy vấn hành trình người dùng
            executeUserJourney(q, hashIdx, sortedIdx);
        }
        else if (subCmd == "resource")
        {
            // Cú pháp: query resource <rid> <t_start> <t_end>
            std::string resourceId = tokens[2];
            long long timeStart = atoll(tokens[3]);
            long long timeEnd = atoll(tokens[4]);

            ResourceJourneyQuery q;
            q.resourceId = resourceId;
            q.timeStart = timeStart;
            q.timeEnd = timeEnd;

            // Chuyển hướng thực thi sang module truy vấn hành trình tài nguyên
            executeResourceJourney(q, hashIdx, sortedIdx);
        }
        else
        {
            printf("[ERROR] Unknown query subcommand: %s\n", subCmd.c_str());
        }
    }
    // Phân vùng logic 2: Xử lý nhóm lệnh thống kê tần suất "top"
    else if (cmd == "top" && tokenCount >= 4)
    {
        if (std::string(tokens[1]) == "resources")
        {
            // Cú pháp: top resources <t_start> <t_end>
            long long timeStart = atoll(tokens[2]);
            long long timeEnd = atoll(tokens[3]);

            // Thực thi thống kê Top 10 tài nguyên được truy cập nhiều nhất trong khoảng thời gian
            executeTopResources(timeStart, timeEnd, sortedIdx, 10);
        }
        else
        {
            printf("[ERROR] Unknown top subcommand: %s\n", tokens[1]);
        }
    }
    // Phân vùng logic 3: Thông báo lỗi sai cú pháp hệ thống
    else
    {
        printf("[ERROR] Invalid query format. Expected:\n");
        printf("  query user <uid> <t_start> <t_end>\n");
        printf("  query resource <rid> <t_start> <t_end>\n");
        printf("  top resources <t_start> <t_end>\n");
    }

    // Giải phóng bộ nhớ vùng đệm C-string tạm thời
    delete[] inputCopy;
}