/**
 * @file        BehaviorDetector.h
 * @brief       Định nghĩa các hàm phát hiện hành vi bất thường dựa trên vị trí
 * địa lý.
 *
 * Cung cấp giải thuật phân tích và xác định các dấu hiệu xâm nhập tài khoản
 * thông qua vị trí dựa trên cơ chế di chuyển không hợp lệ (Impossible travel)
 * và đổi vị trí liên tục (Location churning).
 */

#ifndef BEHAVIOR_DETECTOR_H
#define BEHAVIOR_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"

/**
 * @brief Phát hiện hành vi di chuyển không khả thi về mặt vật lý (Impossible
 * travel).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện
 * được
 * @param hashIdx Chỉ mục băm chứa thông tin phân loại log hệ thống
 * @note Tiêu chí: Một người dùng xuất hiện ở 2 vị trí địa lý khác nhau trong
 * vòng 120 phút.
 */
void detectImpossibleTravel(AnomalyList &results, const HashIndex &hashIdx);

/**
 * @brief Phát hiện hành vi thay đổi vị trí liên tục (Location churning).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện
 * được
 * @param hashIdx Chỉ mục băm chứa thông tin phân loại log hệ thống
 * @note Tiêu chí: Người dùng thay đổi vị trí liên tiếp > 3 lần trong khung thời
 * gian 60 phút.
 */
void detectLocationChurning(AnomalyList &results, const HashIndex &hashIdx);

#endif // BEHAVIOR_DETECTOR_H