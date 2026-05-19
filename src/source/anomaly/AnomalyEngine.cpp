/**
 * @file        AnomalyEngine.cpp
 * @brief       Implementation của bộ điều phối chính và định dạng báo cáo an
 * ninh hệ thống.
 */

#include "anomaly/AnomalyEngine.h"

#include <iomanip>
#include <iostream>

#include "anomaly/AdvancedDetector.h"
#include "anomaly/BehaviorDetector.h"
#include "anomaly/SessionDetector.h"
#include "anomaly/ThresholdDetector.h"
#include "session/SessionBuilder.h"

// ================================================================================
//  Helper structures & functions
// ================================================================================

/**
 * @struct UserAnomalyCount
 * @brief  Cấu trúc nội bộ hỗ trợ thống kê số lượng bất thường theo từng User.
 */
struct UserAnomalyCount {
    std::string userId;  ///< Mã định danh người dùng
    int count;           ///< Số lượng cảnh báo bất thường ghi nhận được
};

/**
 * @brief  Chuyển đổi kiểu enum AnomalyType sang chuỗi ký tự hiển thị trên báo
 * cáo.
 * @note   Hàm nội bộ, chỉ dùng trong file này.
 * @param  type Loại bất thường
 * @return Tên mã của loại bất thường dưới dạng string
 */
static std::string getAnomalyTypeName(AnomalyType type) {
    switch (type) {
        case AnomalyType::MULTI_DEVICE_LOGIN:
            return "A01_MULTI_DEVICE";
        case AnomalyType::CONSECUTIVE_FAILED_LOGIN:
            return "A02_FAILED_LOGIN";
        case AnomalyType::RESOURCE_FLOOD:
            return "A03_RESOURCE_FLOOD";
        case AnomalyType::OFF_HOURS_ACCESS:
            return "A04_OFF_HOURS";
        case AnomalyType::IMPOSSIBLE_TRAVEL:
            return "A05_IMPOSSIBLE_TRAVEL";
        case AnomalyType::LOCATION_CHURNING:
            return "A06_LOCATION_CHURNING";
        case AnomalyType::LONG_SESSION:
            return "A07_LONG_SESSION";
        case AnomalyType::SESSION_FLOOD:
            return "A08_SESSION_FLOOD";
        case AnomalyType::DANGEROUS_SEQUENCE:
            return "A09_DANGEROUS_SEQUENCE";
        case AnomalyType::BRUTE_FORCE:
            return "A10_BRUTE_FORCE";
        case AnomalyType::DORMANT_ACTIVATION:
            return "A11_DORMANT_ACTIVATION";
        case AnomalyType::PRIVILEGE_ESCALATION:
            return "A12_PRIVILEGE_ESCALATION";
        case AnomalyType::DATA_EXFILTRATION:
            return "A13_DATA_EXFILTRATION";
        case AnomalyType::LATERAL_MOVEMENT:
            return "A14_LATERAL_MOVEMENT";
        default:
            return "UNKNOWN";
    }
}

/**
 * @brief  Hàm trộn (Merge) hai mảng con đã sắp xếp cho thuật toán Merge Sort.
 * @note   Hàm nội bộ, sắp xếp tăng dần theo trường timestamp.
 */
static void mergeAnomalies(AnomalyRecord** arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    AnomalyRecord** L = new AnomalyRecord*[n1];
    AnomalyRecord** R = new AnomalyRecord*[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i]->timestamp <= R[j]->timestamp)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

/**
 * @brief  Sắp xếp danh sách bất thường sử dụng thuật toán Merge Sort.
 * @note   Hàm nội bộ, độ phức tạp O(N log N) đảm bảo hiệu năng khi lượng log
 * lớn.
 */
static void sortAnomalies(AnomalyRecord** arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sortAnomalies(arr, left, mid);
        sortAnomalies(arr, mid + 1, right);
        mergeAnomalies(arr, left, mid, right);
    }
}

/**
 * @brief  Hàm trộn (Merge) hai mảng con chứa con trỏ trỏ tới chuỗi mã người dùng.
 * @note   Hàm nội bộ tối ưu hiệu năng gom nhóm (grouping).
 */
static void mergeUserIds(const std::string** arr, const std::string** temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (*arr[i] <= *arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

/**
 * @brief  Sắp xếp danh sách con trỏ chuỗi mã người dùng bằng Merge Sort.
 * @note   Hàm nội bộ đạt độ phức tạp O(N log N) tránh cấp phát tĩnh/động lặp lại.
 */
static void sortUserIds(const std::string** arr, const std::string** temp, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sortUserIds(arr, temp, left, mid);
        sortUserIds(arr, temp, mid + 1, right);
        mergeUserIds(arr, temp, left, mid, right);
    }
}

/**
 * @brief  Xác định mức độ nghiêm trọng (Risk Level) của từng loại bất thường.
 * @note   Hàm nội bộ phục vụ việc phân loại dữ liệu trên báo cáo.
 * @param  type Loại bất thường
 * @return Chuỗi ký tự thể hiện mức độ nguy hiểm (CRITICAL, HIGH, MEDIUM, LOW)
 */
static const char* getRiskLevel(AnomalyType type) {
    switch (type) {
        case AnomalyType::MULTI_DEVICE_LOGIN:
        case AnomalyType::CONSECUTIVE_FAILED_LOGIN:
        case AnomalyType::IMPOSSIBLE_TRAVEL:
        case AnomalyType::BRUTE_FORCE:
        case AnomalyType::PRIVILEGE_ESCALATION:
            return "CRITICAL";
        case AnomalyType::RESOURCE_FLOOD:
        case AnomalyType::LOCATION_CHURNING:
        case AnomalyType::SESSION_FLOOD:
        case AnomalyType::DANGEROUS_SEQUENCE:
        case AnomalyType::DATA_EXFILTRATION:
        case AnomalyType::LATERAL_MOVEMENT:
            return "HIGH";
        case AnomalyType::LONG_SESSION:
        case AnomalyType::DORMANT_ACTIVATION:
            return "MEDIUM";
        case AnomalyType::OFF_HOURS_ACCESS:
            return "LOW";
        default:
            return "UNKNOWN";
    }
}

// ================================================================================
//  Public functions
// ================================================================================

void runAnomalyDetection(AnomalyList& results, const DataStore& store, const HashIndex& hashIdx, const std::string& typeFilter) {
    AnomalyList rawResults;
    initAnomalyList(rawResults);

    bool runThreshold = (typeFilter == "all" || typeFilter == "threshold");
    bool runBehavior  = (typeFilter == "all" || typeFilter == "behavior");
    bool runSession   = (typeFilter == "all" || typeFilter == "session");
    bool runAdvanced  = (typeFilter == "all" || typeFilter == "advanced");

    if (runThreshold) {
        std::cout << "[INFO] Running threshold detectors..." << std::endl;
        detectMultiDeviceLogin(rawResults, hashIdx);
        detectConsecutiveFailedLogin(rawResults, hashIdx);
        detectResourceFlood(rawResults, hashIdx);
        detectOffHoursAccess(rawResults, store);
    }

    if (runBehavior) {
        std::cout << "[INFO] Running behavior detectors..." << std::endl;
        detectImpossibleTravel(rawResults, hashIdx);
        detectLocationChurning(rawResults, hashIdx);
    }

    if (runSession) {
        std::cout << "[INFO] Building sessions..." << std::endl;
        SessionList sessions;
        buildSessions(sessions, hashIdx);

        std::cout << "[INFO] Running session detectors..." << std::endl;
        detectLongSession(rawResults, sessions);
        detectSessionFlood(rawResults, sessions);
        detectDangerousSequence(rawResults, sessions);

        clearSessions(sessions);
    }

    if (runAdvanced) {
        std::cout << "[INFO] Running advanced detectors..." << std::endl;
        detectBruteForce(rawResults, hashIdx);
        detectDormantActivation(rawResults, hashIdx);
        detectPrivilegeEscalation(rawResults, hashIdx);
        detectDataExfiltration(rawResults, hashIdx);
        detectLateralMovement(rawResults, hashIdx);
    }

    std::cout << "[INFO] Sorting " << rawResults.count << " anomalies..." << std::endl;
    // 5. Đồng bộ dòng thời gian trước khi tiến hành tối ưu hóa kết quả
    if (rawResults.count > 1) {
        sortAnomalies(rawResults.records, 0, rawResults.count - 1);
    }

    std::cout << "[INFO] Deduping anomalies..." << std::endl;
    // 6. Loại bỏ nhiễu/trùng lặp (Deduplication)
    // Quy tắc: Cùng user + cùng loại vi phạm + khoảng cách phát hiện < 1 giờ
    // (3600s) -> gộp làm một
    for (int i = 0; i < rawResults.count; i++) {
        AnomalyRecord* curr = rawResults.records[i];
        bool isDuplicate = false;

        // Quét ngược lại các bản ghi đã được chấp thuận trong kết quả tối ưu
        for (int j = results.count - 1; j >= 0; j--) {
            AnomalyRecord* prev = results.records[j];
            if (curr->timestamp - prev->timestamp > 3600)
                break;  // Vượt quá khung thời gian 1 giờ, chắc chắn không trùng

            if (curr->userId == prev->userId && curr->type == prev->type) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            AnomalyRecord* copy = new AnomalyRecord(*curr);
            pushAnomaly(results, copy);
        }
    }

    clearAnomalyList(rawResults);
}

void printAnomalyReport(const AnomalyList& results) {
    std::cout << "\n============================================================="
                 "==================\n";
    std::cout << "                      ANOMALY DETECTION REPORT\n";
    std::cout << "==============================================================="
                 "================\n";
    std::cout << "Total anomalies found: " << results.count << "\n\n";

    if (results.count == 0) {
        std::cout << "No anomalies detected. System is safe.\n";
        std::cout << "============================================================="
                     "==================\n";
        return;
    }

    // Thống kê số lượng theo loại bất thường
    int typeCounts[14] = {0};
    for (int i = 0; i < results.count; i++) {
        int typeIdx = static_cast<int>(results.records[i]->type);
        if (typeIdx >= 0 && typeIdx < 14) typeCounts[typeIdx]++;
    }

    // In phân rã tổng hợp theo loại hành vi
    std::cout << "ANOMALY SUMMARY BY TYPE\n";
    std::cout << "───────────────────────────────────────────────────────────────"
                 "────────────────\n";
    std::cout << std::left << std::setw(25) << "TYPE" << std::setw(8) << "COUNT"
              << "RISK\n";
    std::cout << "───────────────────────────────────────────────────────────────"
                 "────────────────\n";

    for (int i = 0; i < 14; i++) {
        if (typeCounts[i] > 0) {
            AnomalyType type = static_cast<AnomalyType>(i);
            std::cout << std::left << std::setw(25) << getAnomalyTypeName(type) << std::setw(8) << typeCounts[i]
                      << getRiskLevel(type) << "\n";
        }
    }

    // Thống kê tần suất vi phạm của từng User bằng cách sắp xếp O(N log N) và gom nhóm O(N)
    UserAnomalyCount* userCounts = new UserAnomalyCount[results.count];
    int userCountSize = 0;

    if (results.count > 0) {
        const std::string** userIds = new const std::string*[results.count];
        const std::string** temp = new const std::string*[results.count];
        for (int i = 0; i < results.count; i++) {
            userIds[i] = &results.records[i]->userId;
        }

        // Sắp xếp các con trỏ chuỗi mã định danh người dùng
        sortUserIds(userIds, temp, 0, results.count - 1);

        // Gom nhóm và cộng dồn các phần tử trùng lặp liên tục
        std::string currentUserId = *userIds[0];
        int currentCount = 1;

        for (int i = 1; i < results.count; i++) {
            if (*userIds[i] == currentUserId) {
                currentCount++;
            } else {
                userCounts[userCountSize].userId = currentUserId;
                userCounts[userCountSize].count = currentCount;
                userCountSize++;

                currentUserId = *userIds[i];
                currentCount = 1;
            }
        }
        // Ghi nhận nhóm người dùng cuối cùng
        userCounts[userCountSize].userId = currentUserId;
        userCounts[userCountSize].count = currentCount;
        userCountSize++;

        // Giải phóng bộ nhớ đệm tạm thời dùng cho việc sắp xếp
        delete[] userIds;
        delete[] temp;
    }

    // Sắp xếp nổi bọt chọn lọc (Selection sort) lấy Top 10 đối tượng nghi vấn
    // nhất (giảm dần)
    for (int i = 0; i < userCountSize && i < 10; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < userCountSize; j++) {
            if (userCounts[j].count > userCounts[maxIdx].count) maxIdx = j;
        }
        UserAnomalyCount tmp = userCounts[i];
        userCounts[i] = userCounts[maxIdx];
        userCounts[maxIdx] = tmp;
    }

    // Xuất danh sách các tài khoản có nguy cơ cao nhất
    std::cout << "\nTOP " << (userCountSize < 10 ? userCountSize : 10) << " SUSPICIOUS USERS\n";
    std::cout << "───────────────────────────────────────────────────────────────"
                 "────────────────\n";
    std::cout << std::left << std::setw(8) << "RANK" << std::setw(12) << "USER ID"
              << "ANOMALY COUNT\n";
    std::cout << "───────────────────────────────────────────────────────────────"
                 "────────────────\n";

    int topCount = userCountSize < 10 ? userCountSize : 10;
    for (int i = 0; i < topCount; i++) {
        std::cout << std::left << std::setw(8) << (i + 1) << std::setw(12) << userCounts[i].userId
                  << userCounts[i].count << "\n";
    }

    delete[] userCounts;
    std::cout << "==============================================================="
                 "================\n";
}