/**
 * @file        SessionDetector.h
 * @brief       Định nghĩa các hàm phát hiện hành vi bất thường dựa trên phiên
 * làm việc (Session).
 *
 * File này chứa các giải thuật phân tích chuỗi sự kiện được nhóm theo phiên để
 * phát hiện các dấu hiệu bất thường như phiên kéo dài, spam phiên hoặc chuỗi
 * thao tác nguy hiểm.
 */

#ifndef SESSION_DETECTOR_H
#define SESSION_DETECTOR_H

#include "anomaly/AnomalyRecord.h"
#include "session/SessionBuilder.h"

/**
 * @brief Phát hiện phiên làm việc kéo dài bất thường (Long session).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện
 * được
 * @param sessionList Danh sách các phiên làm việc đã được hệ thống xây dựng
 * @note Tiêu chí: Một phiên làm việc duy trì liên tục vượt quá 8 tiếng
 * (MAX_SESSION_HOURS).
 */
void detectLongSession(AnomalyList &results, const SessionList &sessionList);

/**
 * @brief Phát hiện hành vi khởi tạo phiên liên tục (Session flood).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện
 * được
 * @param sessionList Danh sách các phiên làm việc đã được hệ thống xây dựng
 * @note Tiêu chí: Một người dùng khởi tạo > 5 phiên làm việc trong vòng 60
 * phút.
 */
void detectSessionFlood(AnomalyList &results, const SessionList &sessionList);

/**
 * @brief Phát hiện chuỗi thao tác thực thi nguy hiểm (Dangerous sequence).
 * @param results Danh sách thu nhận các bản ghi hành vi bất thường phát hiện
 * được
 * @param sessionList Danh sách các phiên làm việc đã được hệ thống xây dựng
 * @note Tiêu chí: Xuất hiện hành vi DOWNLOAD trong vòng 10 phút ngay sau khi có
 * ADMIN_ACTION.
 */
void detectDangerousSequence(AnomalyList &results,
                             const SessionList &sessionList);

#endif // SESSION_DETECTOR_H