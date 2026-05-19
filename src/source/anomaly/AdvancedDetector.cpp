/**
 * @file        AdvancedDetector.cpp
 * @brief       Implementation của các thuật toán phát hiện hành vi bất thường nâng cao.
 *
 * Cung cấp các cơ chế phân tích chuỗi sự kiện theo thời gian thực hoặc theo lô (batch)
 * dựa trên các chỉ mục băm đa chiều để nhận diện Brute Force, tài khoản ngủ đông kích hoạt bất thường,
 * leo thang đặc quyền, rò rỉ dữ liệu và dịch chuyển ngang (Lateral Movement).
 */

#include "anomaly/AdvancedDetector.h"

#include "anomaly/AnomalyConfig.h"

// ================================================================================
//  Public functions
// ================================================================================

void detectBruteForce(AnomalyList& results, const HashIndex& hashIdx) {
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++) {
        HashNode* node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr) {
            int failedStreak = 0;

            for (int i = 0; i < node->count; i++) {
                LogRecord* event = node->values[i];

                if (event->event_type == EventType::FAILED_LOGIN) {
                    failedStreak++;
                } else if (event->event_type == EventType::LOGIN) {
                    if (failedStreak >= AnomalyConfig::MAX_FAILED_LOGIN_STREAK) {
                        AnomalyRecord* rec = new AnomalyRecord();
                        rec->type = AnomalyType::BRUTE_FORCE;
                        rec->userId = node->key;
                        rec->timestamp = event->timestamp;
                        rec->detail = std::to_string(failedStreak) + " fails -> success [BONUS]";
                        pushAnomaly(results, rec);
                    }
                    failedStreak = 0;  // Reset chuỗi đếm sau khi đăng nhập thành công
                } else {
                    // Các sự kiện trung gian khác cũng làm đứt chuỗi brute force ngắt quãng
                    failedStreak = 0;
                }
            }
            node = node->next;
        }
    }
}

void detectDormantActivation(AnomalyList& results, const HashIndex& hashIdx) {
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++) {
        HashNode* node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr) {
            if (node->count < 2) {
                node = node->next;
                continue;
            }

            for (int i = 1; i < node->count; i++) {
                LogRecord* prevEvent = node->values[i - 1];
                LogRecord* currentEvent = node->values[i];

                long long gap = currentEvent->timestamp - prevEvent->timestamp;
                if (gap > (AnomalyConfig::DORMANT_DAYS * 86400))  // Quy đổi ngày ra giây (1 ngày = 86400s)
                {
                    // Tài khoản đã ngủ đông vượt ngưỡng. Tiến hành đếm số lượng event phát sinh trong 1 giờ kế tiếp.
                    int burstCount = 0;
                    for (int j = i; j < node->count; j++) {
                        if (node->values[j]->timestamp - currentEvent->timestamp > 3600) {
                            break;  // Vượt quá cửa sổ trượt 1 giờ (3600s)
                        }
                        burstCount++;
                    }

                    if (burstCount > AnomalyConfig::BURST_EVENTS_PER_HOUR) {
                        AnomalyRecord* rec = new AnomalyRecord();
                        rec->type = AnomalyType::DORMANT_ACTIVATION;
                        rec->userId = node->key;
                        rec->timestamp = currentEvent->timestamp;
                        rec->detail = "Dormant for " + std::to_string(gap / 86400) + " days, then " +
                                      std::to_string(burstCount) + " events in 1h [BONUS]";
                        pushAnomaly(results, rec);
                    }
                }
            }
            node = node->next;
        }
    }
}

void detectPrivilegeEscalation(AnomalyList& results, const HashIndex& hashIdx) {
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++) {
        HashNode* node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr) {
            for (int i = 0; i < node->count; i++) {
                LogRecord* currentEvent = node->values[i];
                if (currentEvent->event_type == EventType::ADMIN_ACTION) {
                    // Quét ngược dòng thời gian tối đa 5 phút (300 giây) để tìm dấu vết FAILED_LOGIN
                    for (int j = i - 1; j >= 0; j--) {
                        LogRecord* pastEvent = node->values[j];
                        if (currentEvent->timestamp - pastEvent->timestamp > 300) {
                            break;  // Đã vượt ra khỏi cửa sổ giám sát 5 phút
                        }
                        if (pastEvent->event_type == EventType::FAILED_LOGIN) {
                            AnomalyRecord* rec = new AnomalyRecord();
                            rec->type = AnomalyType::PRIVILEGE_ESCALATION;
                            rec->userId = node->key;
                            rec->timestamp = currentEvent->timestamp;
                            rec->detail = "ADMIN_ACTION within 5 min after FAILED_LOGIN [BONUS]";
                            pushAnomaly(results, rec);
                            break;  // Giới hạn cảnh báo tối đa một lần cho mỗi sự kiện ADMIN_ACTION
                        }
                    }
                }
            }
            node = node->next;
        }
    }
}

void detectDataExfiltration(AnomalyList& results, const HashIndex& hashIdx) {
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++) {
        HashNode* node = hashIdx.byUser.buckets[bucket];
        while (node != nullptr) {
            for (int i = 0; i < node->count; i++) {
                LogRecord* currentEvent = node->values[i];
                if (currentEvent->event_type != EventType::DOWNLOAD) {
                    continue;
                }

                // Tích lũy số lượng sự kiện DOWNLOAD trong vòng 5 phút (300 giây) trở về trước
                int downloadCount = 0;
                for (int j = i; j >= 0; j--) {
                    LogRecord* pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 300) {
                        break;  // Đã vượt ra ngoài phạm vi 5 phút
                    }
                    if (pastEvent->event_type == EventType::DOWNLOAD) {
                        downloadCount++;
                    }
                }

                if (downloadCount > 10) {
                    AnomalyRecord* rec = new AnomalyRecord();
                    rec->type = AnomalyType::DATA_EXFILTRATION;
                    rec->userId = node->key;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail =
                        "User performed " + std::to_string(downloadCount) + " DOWNLOAD events in 5 min [BONUS]";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}

void detectLateralMovement(AnomalyList& results, const HashIndex& hashIdx) {
    for (int bucket = 0; bucket < hashIdx.byDevice.tableSize; bucket++) {
        HashNode* node = hashIdx.byDevice.buckets[bucket];
        while (node != nullptr) {
            for (int i = 0; i < node->count; i++) {
                LogRecord* currentEvent = node->values[i];

                // Thiết lập cấu trúc lưu trữ và cửa sổ trượt 24 giờ để đếm số lượng tài khoản duy nhất
                std::string uniqueUsers[10];
                int uniqueCount = 0;

                for (int j = i; j >= 0; j--) {
                    LogRecord* pastEvent = node->values[j];
                    if (currentEvent->timestamp - pastEvent->timestamp > 86400) {
                        break;  // Đã vượt quá phạm vi cửa sổ 24 giờ
                    }

                    // Kiểm tra tính duy nhất của User trong danh sách tạm thời
                    bool userExists = false;
                    for (int u = 0; u < uniqueCount; u++) {
                        if (uniqueUsers[u] == pastEvent->user_id) {
                            userExists = true;
                            break;
                        }
                    }

                    if (!userExists) {
                        if (uniqueCount < 10) {
                            uniqueUsers[uniqueCount++] = pastEvent->user_id;
                        } else {
                            uniqueCount++;  // Tiếp tục tăng biến đếm để phản ánh chính xác dữ liệu thực tế
                        }
                    }
                }

                if (uniqueCount > 2) {
                    AnomalyRecord* rec = new AnomalyRecord();
                    rec->type = AnomalyType::LATERAL_MOVEMENT;
                    rec->userId = currentEvent->user_id;
                    rec->timestamp = currentEvent->timestamp;
                    rec->detail = "Device " + node->key + " accessed by " + std::to_string(uniqueCount) +
                                  " different users in 24h [BONUS]";
                    pushAnomaly(results, rec);
                }
            }
            node = node->next;
        }
    }
}