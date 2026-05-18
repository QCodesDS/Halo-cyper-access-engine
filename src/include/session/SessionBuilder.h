/**
 * @file        SessionBuilder.h
 * @brief       Định nghĩa cấu trúc dữ liệu và các hàm quản lý phiên hoạt động
 * của người dùng (User Session).
 *
 * Cung cấp giải pháp gom cụm và phân tích chuỗi log thô theo từng phiên hoạt
 * động liên tục dựa trên các quy tắc cấu hình hệ thống (Thời gian hết hạn
 * phiên, sự kiện Đăng nhập/Đăng xuất).
 */

#ifndef SESSION_BUILDER_H
#define SESSION_BUILDER_H

#include "indexing/HashIndex.h"
#include "models/LogRecord.h"

/**
 * @struct UserSession
 * @brief  Cấu trúc lưu trữ thông tin của một phiên hoạt động liên tục từ một
 * người dùng.
 */
struct UserSession {
  std::string userId; ///< Mã định danh duy nhất của người dùng sở hữu phiên
  long long
      startTime; ///< Nhãn thời gian bắt đầu phiên hoạt động (Unix Timestamp)
  long long
      endTime; ///< Nhãn thời gian kết thúc phiên hoạt động (Unix Timestamp)

  LogRecord **events; ///< Mảng động chứa các con trỏ trỏ đến các sự kiện thuộc
                      ///< phiên này
  int eventCount;    ///< Số lượng sự kiện hiện có trong phiên
  int eventCapacity; ///< Sức chứa tối đa hiện tại của mảng sự kiện
};

/**
 * @struct SessionList
 * @brief  Cấu trúc danh sách quản lý tập trung tất cả các phiên hoạt động của
 * toàn bộ người dùng.
 */
struct SessionList {
  UserSession **
      sessions; ///< Mảng động chứa các con trỏ quản lý các thực thể UserSession
  int count;    ///< Tổng số lượng phiên hiện có trong danh sách
  int capacity; ///< Sức chứa tối đa hiện tại của mảng danh sách phiên
};

/**
 * @brief  Trích xuất dữ liệu từ HashIndex theo từng người dùng để phân tách
 * thành các phiên hoạt động.
 * @note   Thuật toán duyệt qua cấu trúc bảng băm chỉ mục người dùng, nhận diện
 * ranh giới phiên qua mốc thời gian quá hạn (Session Timeout) hoặc các tín hiệu
 * sự kiện LOGIN / LOGOUT đặc biệt.
 * @param  sessionList Tham chiếu tới danh sách quản lý phiên cần nạp kết quả
 * @param  hashIdx Hệ thống chỉ mục băm đa chiều đã được xây dựng hoàn chỉnh từ
 * trước
 */
void buildSessions(SessionList &sessionList, const HashIndex &hashIdx);

/**
 * @brief  Giải phóng toàn bộ vùng nhớ cấp phát động cho danh sách phiên và các
 * mảng sự kiện nội bộ.
 * @warning Hàm này thu hồi vùng nhớ của các cấu trúc quản lý cấu trúc phiên
 * (`UserSession`), quyền sở hữu của các bản ghi `LogRecord` gốc vẫn thuộc về
 * DataStore.
 * @param  sessionList Tham chiếu tới danh sách phiên cần giải phóng bộ nhớ
 */
void clearSessions(SessionList &sessionList);

#endif // SESSION_BUILDER_H