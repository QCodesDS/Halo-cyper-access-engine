/**
 * @file        LogRecord.h
 * @brief       Đại diện cho một bản ghi nhật ký an ninh hệ thống.
 *
 * Cấu trúc này được thiết kế để lưu trữ thông tin sự kiện truy cập tài nguyên
 * một cách hiệu quả, hỗ trợ logging, audit trail và phân tích bảo mật.
 */

#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include "models/EventType.h"
#include "models/Location.h"

/**
 * @struct LogRecord
 * @brief  Đại diện cho một bản ghi nhật ký an ninh hệ thống.
 *
 * Cấu trúc này được tối ưu hóa về mặt bộ nhớ và hiệu năng cho việc ghi log
 * các hoạt động truy cập tài nguyên từ người dùng và thiết bị.
 */
struct LogRecord {
  std::string user_id;     ///< ID người dùng (ví dụ: "U007")
  std::string device_id;   ///< ID thiết bị (ví dụ: "D018")
  std::string app_id;      ///< ID ứng dụng (ví dụ: "APP003")
  std::string resource_id; ///< ID tài nguyên được truy cập (ví dụ: "R025")
  EventType event_type;    ///< Loại sự kiện
  Location location;       ///< Vị trí địa lý phát sinh sự kiện
  long long timestamp;     ///< Thời gian Unix timestamp (giây)

  /// Constructor mặc định
  LogRecord();

  /**
   * @brief Constructor khởi tạo đầy đủ các trường dữ liệu.
   * @param uid ID người dùng
   * @param did ID thiết bị
   * @param aid ID ứng dụng
   * @param rid ID tài nguyên
   * @param et  Loại sự kiện
   * @param loc Vị trí địa lý
   * @param ts  Thời gian Unix timestamp
   */
  LogRecord(const std::string &uid, const std::string &did,
            const std::string &aid, const std::string &rid, EventType et,
            Location loc, long long ts);
};

/**
 * @brief In thông tin LogRecord ra console (dùng cho debug và development).
 * @param record Bản ghi cần in
 */
void printRecord(const LogRecord &record);

#endif // LOG_RECORD_H