/**
 * @file        Location.h
 * @brief       Định nghĩa các mã quốc gia / khu vực địa lý.
 *
 * Enum này được sử dụng để ghi nhận vị trí địa lý phát sinh sự kiện
 * trong hệ thống log an ninh.
 */

#ifndef LOCATION_H
#define LOCATION_H

#include <string>

/**
 * @enum Location
 * @brief Các quốc gia / khu vực địa lý được hỗ trợ trong hệ thống.
 */
enum Location {
  US,              ///< Hoa Kỳ
  VN,              ///< Việt Nam
  JP,              ///< Nhật Bản
  KR,              ///< Hàn Quốc
  SG,              ///< Singapore
  CN,              ///< Trung Quốc
  DE,              ///< Đức
  FR,              ///< Pháp
  UK,              ///< Vương quốc Anh
  AU,              ///< Úc
  CA,              ///< Canada
  IN,              ///< Ấn Độ
  BR,              ///< Brazil
  RU,              ///< Nga
  TH,              ///< Thái Lan
  UNKNOWN_LOCATION ///< Không xác định
};

/**
 * @brief Chuyển chuỗi thành Location (không phân biệt hoa thường).
 * @param s Chuỗi mã quốc gia (ví dụ: "VN", "US", "JP")
 * @return Location tương ứng, UNKNOWN_LOCATION nếu không khớp
 */
Location parseLocation(const std::string &s);

/**
 * @brief Chuyển Location thành chuỗi string.
 * @param loc Mã vị trí địa lý
 * @return Tên mã quốc gia dưới dạng string
 */
std::string locationToString(Location loc);

#endif // LOCATION_H