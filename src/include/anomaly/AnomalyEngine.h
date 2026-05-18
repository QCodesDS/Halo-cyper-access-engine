/**
 * @file        AnomalyEngine.h
 * @brief       Định nghĩa bộ điều phối chính (Engine) cho quá trình quét và báo cáo bất thường.
 *
 * Engine chịu trách nhiệm gọi tuần tự các bộ dò tìm (Detectors), tập hợp dữ liệu,
 * sắp xếp theo dòng thời gian, loại bỏ các cảnh báo trùng lặp và xuất báo cáo an ninh.
 */

#ifndef ANOMALY_ENGINE_H
#define ANOMALY_ENGINE_H

#include "anomaly/AnomalyRecord.h"
#include "indexing/HashIndex.h"
#include "storage/DataStore.h"

/**
 * @brief Chạy toàn bộ các Detector và trả về danh sách Anomaly đã được lọc trùng.
 * @param results Danh sách đích thu nhận các bản ghi bất thường (đã được sắp xếp và lọc trùng)
 * @param store Cơ sở dữ liệu lưu trữ log hệ thống dạng tuần tự
 * @param hashIdx Chỉ mục băm hỗ trợ truy cập nhanh theo User/Device
 */
void runAnomalyDetection(AnomalyList &results, const DataStore &store, const HashIndex &hashIdx);

/**
 * @brief In báo cáo tổng hợp thông tin bất thường ra màn hình theo định dạng chuẩn.
 * @param results Danh sách các bản ghi bất thường cần kết xuất báo cáo
 */
void printAnomalyReport(const AnomalyList &results);

#endif // ANOMALY_ENGINE_H