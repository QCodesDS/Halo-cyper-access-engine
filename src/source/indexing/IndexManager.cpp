/**
 * @file        IndexManager.cpp
 * @brief       Implementation các hàm điều phối xây dựng chỉ mục tổng thể và đo lường hiệu năng.
 */

#include "indexing/IndexManager.h"
#include <ctime>
#include <iostream>
#include <iomanip>

// ================================================================================
//  Helper functions (Nội bộ)
// ================================================================================

/**
 * @brief  Lựa chọn kích thước bảng băm thích hợp (Ưu tiên số nguyên tố lớn) dựa trên tổng số bản ghi log.
 * @param  recordCount Tổng số lượng bản ghi hiện có trong kho dữ liệu thô
 * @return Kích thước số lượng bucket tối ưu cho thuật toán băm tránh va chạm
 */
static int selectTableSize(int recordCount)
{
    if (recordCount <= 0)
        return 20011; // Số nguyên tố tối thiểu mặc định
    else if (recordCount <= 10000)
        return 20011; // Số nguyên tố phù hợp cho quy mô dưới ~10k bản ghi
    else if (recordCount <= 1000000)
        return 1999993; // Số nguyên tố lớn phù hợp cho quy mô đến ~1 triệu bản ghi
    else
        return 1999993 + (recordCount / 100000); // Mở rộng tuyến tính khi dữ liệu cực lớn
}

// ================================================================================
//  Public functions
// ================================================================================

void buildAllIndexes(IndexManager &mgr, DataStore &store)
{
    if (store.count == 0)
    {
        mgr.isBuilt = false;
        return;
    }

    // Ghi nhận thời điểm bắt đầu xây dựng chỉ mục
    clock_t start = clock();

    // Xác định cấu hình kích thước bảng băm tối ưu
    int tableSize = selectTableSize(store.count);

    // Xây dựng hệ thống chỉ mục băm đa chiều
    buildHashIndex(mgr.hashIdx, store, tableSize);

    // Xây dựng chỉ mục mảng sắp xếp tuyến tính theo thời gian
    buildSortedIndex(mgr.sortedIdx, store);

    // Tính toán tổng thời gian thực thi của CPU
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    mgr.isBuilt = true;

    // Xuất nhật ký hoàn thành ra luồng đầu ra tiêu chuẩn
    std::cout << "[INFO] Indexes built in " << std::fixed << std::setprecision(2) << elapsed << "s" << std::endl;
}

void clearAllIndexes(IndexManager &mgr)
{
    clearHashIndex(mgr.hashIdx);
    clearSortedIndex(mgr.sortedIdx);
    mgr.isBuilt = false;
}