/**
 * @file        ThresholdDetector.h
 * @brief       Định nghĩa các hàm phát hiện bất thường dựa trên việc vượt ngưỡng thô (Threshold).
 *
 * File này cung cấp các giải thuật kiểm tra tần suất đơn giản nhằm phát hiện nhanh
 * các hành vi đa thiết bị, dò mật khẩu, spam tài nguyên hoặc truy cập sai khung giờ quy định.
 */

#ifndef THRESHOLD_DETECTOR_H
#define THRESHOLD_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"
#include "storage/DataStore.h"

/**
 * @brief Phát hiện hành vi đăng nhập từ quá nhiều thiết bị (Multi-device login).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa thông tin phân loại log hệ thống
 * @note Tiêu chí: Một tài khoản đăng nhập thành công/thao tác trên > 3 thiết bị khác nhau trong 1 giờ.
 */
void detectMultiDeviceLogin(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện chuỗi đăng nhập thất bại liên tiếp (Consecutive failed login).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa thông tin phân loại log hệ thống
 * @note Tiêu chí: Tài khoản có chuỗi FAILED_LOGIN liên tiếp vượt quá 5 lần.
 */
void detectConsecutiveFailedLogin(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện thiết bị khai thác tài nguyên mật độ cao (Resource flood).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện được
 * @param hashIdx Chỉ mục băm chứa thông tin phân loại log hệ thống
 * @note Tiêu chí: Một thiết bị gửi yêu cầu truy cập > 20 tài nguyên định danh khác nhau trong 30 phút.
 */
void detectResourceFlood(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện hành vi truy cập hệ thống ngoài giờ làm việc (Off-hours access).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện được
 * @param store Cơ sở dữ liệu lưu trữ log hệ thống dạng tuần tự
 * @note Tiêu chí: Phát hiện log phát sinh ngoài ca làm việc tiêu chuẩn (8:00 - 18:00 UTC).
 */
void detectOffHoursAccess(AnomalyList &results, const DataStore &store);

#endif // THRESHOLD_DETECTOR_H