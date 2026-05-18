/**
 * @file        SortedIndex.h
 * @brief       Định nghĩa cấu trúc dữ liệu và các hàm thao tác trên chỉ mục sắp
 * xếp theo thời gian (Sorted Index).
 *
 * Chỉ mục này quản lý một mảng phẳng chứa các con trỏ trỏ đến LogRecord được
 * sắp xếp tăng dần theo nhãn thời gian (timestamp) bằng thuật toán Merge Sort,
 * hỗ trợ tìm kiếm nhị phân khoảng thời gian nhanh chóng.
 */

#ifndef SORTED_INDEX_H
#define SORTED_INDEX_H

#include "models/LogRecord.h"
#include "storage/DataStore.h"

/**
 * @struct SortedIndex
 * @brief  Cấu trúc lưu trữ chỉ mục mảng con trỏ sắp xếp tăng dần theo mốc thời
 * gian.
 */
struct SortedIndex {
  LogRecord *
      *records; ///< Mảng động chứa các con trỏ trỏ tới LogRecord (Đã sắp xếp)
  int count;    ///< Tổng số lượng con trỏ bản ghi có trong chỉ mục
};

/**
 * @brief Xây dựng cấu trúc chỉ mục sắp xếp từ kho dữ liệu tuần tự DataStore
 * bằng thuật toán sắp xếp trộn (Merge Sort).
 * @param idx Tham chiếu tới cấu trúc SortedIndex cần khởi tạo và sắp xếp dữ
 * liệu
 * @param store Kho lưu trữ bản ghi gốc nguồn
 */
void buildSortedIndex(SortedIndex &idx, DataStore &store);

/**
 * @brief Thuật toán sắp xếp trộn đệ quy (Merge Sort) sắp xếp các con trỏ bản
 * ghi theo timestamp tăng dần.
 * @param arr Mảng các con trỏ bản ghi LogRecord cần sắp xếp
 * @param left Chỉ mục phần tử đầu tiên của phân vùng đang xét
 * @param right Chỉ mục phần tử cuối cùng của phân vùng đang xét
 */
void mergeSort(LogRecord **arr, int left, int right);

/**
 * @brief Hàm bổ trợ trộn hai phân vùng đã sắp xếp thành một mảng con hợp nhất
 * ổn định (Stable Merge).
 * @param arr Mảng các con trỏ bản ghi gốc
 * @param left Chỉ mục bắt đầu phân vùng trái
 * @param mid Chỉ mục ranh giới giữa hai phân vùng
 * @param right Chỉ mục kết thúc phân vùng phải
 */
void merge(LogRecord **arr, int left, int mid, int right);

/**
 * @brief Tìm kiếm nhị phân vị trí bản ghi đầu tiên có nhãn thời gian lớn hơn
 * hoặc bằng mốc quy định (>= t).
 * @param idx Tham chiếu tới cấu trúc chỉ mục đã được sắp xếp
 * @param t Giá trị mốc thời gian Unix timestamp làm đích tìm kiếm
 * @return Vị trí index đầu tiên thỏa mãn trong mảng, hoặc trả về giá trị
 * `count` nếu tất cả bản ghi đều < t
 */
int binarySearchStart(const SortedIndex &idx, long long t);

/**
 * @brief Tìm kiếm nhị phân vị trí bản ghi cuối cùng có nhãn thời gian nhỏ hơn
 * hoặc bằng mốc quy định (<= t).
 * @param idx Tham chiếu tới cấu trúc chỉ mục đã được sắp xếp
 * @param t Giá trị mốc thời gian Unix timestamp làm đích tìm kiếm
 * @return Vị trí index cuối cùng thỏa mãn trong mảng, hoặc trả về `-1` nếu tất
 * cả bản ghi đều > t
 */
int binarySearchEnd(const SortedIndex &idx, long long t);

/**
 * @brief Giải phóng cấu trúc mảng con trỏ của chỉ mục (Không xóa thực thể
 * LogRecord bên dưới).
 * @warning Quyền sở hữu và vòng đời của các đối tượng LogRecord thuộc về kho dữ
 * liệu chính DataStore.
 * @param idx Tham chiếu tới cấu trúc chỉ mục cần xóa
 */
void clearSortedIndex(SortedIndex &idx);

#endif // SORTED_INDEX_H