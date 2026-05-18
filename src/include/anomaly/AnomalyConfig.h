/**
 * @file        AnomalyConfig.h
 * @brief       Định nghĩa các cấu hình ngưỡng (threshold) cho hệ thống phát
 * hiện bất thường.
 *
 * Các giá trị hằng số trong namespace này quy định giới hạn an toàn của hệ
 * thống. Mọi hành vi vượt quá các ngưỡng này đều bị coi là dấu hiệu vi phạm an
 * ninh.
 */

#ifndef ANOMALY_CONFIG_H
#define ANOMALY_CONFIG_H

/**
 * @namespace AnomalyConfig
 * @brief     Chứa các hằng số cấu hình phân tích hành vi.
 */
namespace AnomalyConfig {
const int MAX_DEVICES_PER_HOUR =
    3; ///< Số lượng thiết bị tối đa một user được dùng trong 1 giờ
const int MAX_FAILED_LOGIN_STREAK =
    5; ///< Số lần đăng nhập sai liên tiếp tối đa trước khi khóa/cảnh báo
const int MAX_RESOURCES_PER_30MIN =
    20; ///< Số tài nguyên tối đa được truy cập trong 30 phút
const int WORK_HOUR_START = 8; ///< Giờ bắt đầu ca làm việc tiêu chuẩn (UTC)
const int WORK_HOUR_END = 18;  ///< Giờ kết thúc ca làm việc tiêu chuẩn (UTC)
const int IMPOSSIBLE_TRAVEL_MINS =
    120; ///< Ngưỡng thời gian tối thiểu giữa 2 lần đăng nhập ở khoảng cách xa
         ///< (phút)
const int MAX_LOCATION_CHANGES =
    3; ///< Số lần thay đổi vị trí tối đa cho phép trong phiên
const int MAX_SESSION_HOURS =
    8; ///< Thời gian duy trì một phiên làm việc tối đa (giờ)
const int MAX_SESSIONS_PER_HOUR =
    5; ///< Số lượng phiên làm việc tối đa được khởi tạo trong 1 giờ
const int DANGEROUS_SEQ_MINS =
    10; ///< Khung thời gian kiểm tra chuỗi hành vi nguy hiểm (phút)
const int SESSION_TIMEOUT_MINS =
    30; ///< Thời gian hết hạn của một phiên khi không hoạt động (phút)
const int DORMANT_DAYS =
    7; ///< Số ngày tối thiểu để xác định một tài khoản ở trạng thái ngủ đông
const int BURST_EVENTS_PER_HOUR =
    20; ///< Số lượng sự kiện tối đa trong 1 giờ của tài khoản vừa kích hoạt lại
} // namespace AnomalyConfig

#endif // ANOMALY_CONFIG_H