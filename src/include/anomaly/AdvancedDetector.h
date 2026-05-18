/**
 * @file        AdvancedDetector.h
 * @brief       Định nghĩa các hàm phát hiện hành vi bất thường nâng cao.
 *
 * File này cung cấp các giải thuật phân tích chuỗi sự kiện để phát hiện
 * các mối đe dọa an ninh phức tạp như Brute Force, kích hoạt tài khoản ngủ đông,
 * leo thang đặc quyền, rò rỉ dữ liệu và dịch chuyển ngang.
 */

#ifndef ADVANCED_DETECTOR_H
#define ADVANCED_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"

/**
 * @brief Phát hiện hành vi dò đoán mật khẩu (Brute Force).
 * @param results Danh sách lưu trữ các bản ghi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa toàn bộ dữ liệu log hệ thống
 * @note Tiêu chí: Có >= 5 sự kiện FAILED_LOGIN liên tiếp và kết thúc bằng LOGIN thành công.
 */
void detectBruteForce(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện tài khoản ngủ đông hoạt động bất thường trở lại (Dormant user activation).
 * @param results Danh sách lưu trữ các bản ghi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa toàn bộ dữ liệu log hệ thống
 * @note Tiêu chí: Không có hoạt động > 7 ngày, sau đó đột ngột xuất hiện > 20 sự kiện trong 1 giờ.
 */
void detectDormantActivation(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện hành vi leo thang đặc quyền (Privilege escalation).
 * @param results Danh sách lưu trữ các bản ghi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa toàn bộ dữ liệu log hệ thống
 * @note Tiêu chí: Xuất hiện sự kiện ADMIN_ACTION trong vòng 5 phút sau khi có FAILED_LOGIN.
 */
void detectPrivilegeEscalation(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện hành vi rò rỉ hoặc đánh cắp dữ liệu (Data exfiltration).
 * @param results Danh sách lưu trữ các bản ghi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa toàn bộ dữ liệu log hệ thống
 * @note Tiêu chí: Có > 10 sự kiện DOWNLOAD diễn ra trong vòng 5 phút.
 */
void detectDataExfiltration(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện hành vi dịch chuyển ngang trong mạng (Lateral movement).
 * @param results Danh sách lưu trữ các bản ghi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa toàn bộ dữ liệu log hệ thống
 * @note Tiêu chí: Một thiết bị (DeviceID) được chia sẻ và sử dụng bởi nhiều tài khoản người dùng khác nhau.
 */
void detectLateralMovement(AnomalyList &results, const HashIndex &hashIdx);

#endif // ADVANCED_DETECTOR_H