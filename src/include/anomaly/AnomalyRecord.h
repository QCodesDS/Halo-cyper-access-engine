/**
 * @file        AnomalyRecord.h
 * @brief       Định nghĩa các loại hành vi bất thường và cấu trúc lưu trữ dữ
 * liệu cảnh báo.
 *
 * File này chứa các enum phân loại mức độ, phân loại hành vi vi phạm
 * (AnomalyType), cấu trúc lưu trữ bản ghi (AnomalyRecord) và cấu trúc danh sách
 * mảng động (AnomalyList).
 */

#ifndef ANOMALY_RECORD_H
#define ANOMALY_RECORD_H

#include <string>

/**
 * @enum  AnomalyType
 * @brief Các loại hành vi bất thường hoặc vi phạm chính sách an ninh hệ thống.
 */
enum class AnomalyType {
  MULTI_DEVICE_LOGIN, ///< A01: Đăng nhập từ nhiều thiết bị trong thời gian ngắn
  CONSECUTIVE_FAILED_LOGIN, ///< A02: Đăng nhập sai nhiều lần liên tiếp
  RESOURCE_FLOOD,       ///< A03: Khai thác tài nguyên với tần suất bất thường
  OFF_HOURS_ACCESS,     ///< A04: Truy cập ngoài giờ làm việc quy định
  IMPOSSIBLE_TRAVEL,    ///< A05: Di chuyển không khả thi về mặt vật lý
  LOCATION_CHURNING,    ///< A06: Thay đổi vị trí liên tục trong phiên
  LONG_SESSION,         ///< A07: Phiên làm việc kéo dài bất thường
  SESSION_FLOOD,        ///< A08: Khởi tạo quá nhiều phiên làm việc
  DANGEROUS_SEQUENCE,   ///< A09: Chuỗi hành vi độc hại tuần tự
  BRUTE_FORCE,          ///< A10: Dò đoán mật khẩu thành công
  DORMANT_ACTIVATION,   ///< A11: Tài khoản ngủ đông hoạt động mạnh trở lại
  PRIVILEGE_ESCALATION, ///< A12: Leo thang đặc quyền hệ thống - Mức độ CRITICAL
  DATA_EXFILTRATION,    ///< A13: Đánh cắp hoặc rò rỉ dữ liệu - Mức độ HIGH
  LATERAL_MOVEMENT ///< A14: Dịch chuyển ngang chiếm quyền thiết bị - Mức độ
                   ///< HIGH
};

/**
 * @struct AnomalyRecord
 * @brief  Cấu trúc thông tin chi tiết về một sự kiện bất thường được phát hiện.
 */
struct AnomalyRecord {
  AnomalyType type;    ///< Loại hành vi bất thường
  std::string userId;  ///< Mã định danh tài khoản vi phạm
  long long timestamp; ///< Thời điểm ghi nhận hành vi (Unix timestamp)
  std::string detail;  ///< Mô tả chi tiết ngữ cảnh vi phạm
};

/**
 * @struct AnomalyList
 * @brief  Cấu trúc danh sách mảng động quản lý các con trỏ AnomalyRecord.
 */
struct AnomalyList {
  AnomalyRecord **records; ///< Mảng các con trỏ quản lý bản ghi bất thường
  int count;               ///< Số lượng phần tử hiện tại trong danh sách
  int capacity;            ///< Sức chứa tối đa hiện tại của mảng
};

/**
 * @brief Khởi tạo danh sách mảng động chứa các bản ghi bất thường.
 * @param list Tham chiếu tới cấu trúc danh sách cần khởi tạo
 * @param capacity Sức chứa ban đầu được cấp phát (mặc định là 100)
 */
void initAnomalyList(AnomalyList &list, int capacity = 100);

/**
 * @brief Thêm một bản ghi bất thường mới vào danh sách (Tự động tăng kích thước
 * mảng nếu đầy).
 * @param list Tham chiếu tới danh sách đích
 * @param record Con trỏ vùng nhớ chứa bản ghi bất thường cần thêm
 */
void pushAnomaly(AnomalyList &list, AnomalyRecord *record);

/**
 * @brief Giải phóng toàn bộ bộ nhớ của các bản ghi và cấu trúc danh sách.
 * @param list Tham chiếu tới danh sách cần xóa và thu hồi bộ nhớ
 */
void clearAnomalyList(AnomalyList &list);

#endif // ANOMALY_RECORD_H