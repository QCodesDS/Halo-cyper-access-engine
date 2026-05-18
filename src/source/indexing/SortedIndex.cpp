/**
 * @file        SortedIndex.cpp
 * @brief       Implementation giải thuật Merge Sort ổn định và cơ chế tìm kiếm nhị phân trên SortedIndex.
 */

#include "indexing/SortedIndex.h"
#include <cstring>

// ================================================================================
//  Public functions
// ================================================================================

void merge(LogRecord **arr, int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Cấp phát các mảng phụ tạm thời để lưu trữ dữ liệu hai phân vùng
    LogRecord **leftArr = new LogRecord *[leftSize];
    LogRecord **rightArr = new LogRecord *[rightSize];

    // Sao chép dữ liệu trích xuất từ mảng gốc vào mảng tạm
    for (int i = 0; i < leftSize; i++)
    {
        leftArr[i] = arr[left + i];
    }

    for (int j = 0; j < rightSize; j++)
    {
        rightArr[j] = arr[mid + 1 + j];
    }

    // Tiến hành trộn (merge) hai mảng tạm tuần tự trở lại mảng gốc arr
    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize)
    {
        // Sử dụng toán tử toán học `<=` để đảm bảo tính sắp xếp ổn định (Stable Sort)
        if (leftArr[i]->timestamp <= rightArr[j]->timestamp)
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Sao chép các phần tử còn dư lại của mảng tạm bên trái (nếu có)
    while (i < leftSize)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Sao chép các phần tử còn dư lại của mảng tạm bên phải (nếu có)
    while (j < rightSize)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    // Thu hồi vùng nhớ tạm thời của hai mảng phụ
    delete[] leftArr;
    delete[] rightArr;
}

void mergeSort(LogRecord **arr, int left, int right)
{
    if (left < right)
    {
        // Tính toán chỉ mục chia đôi tránh hiện tượng tràn số nguyên (Overflow)
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void buildSortedIndex(SortedIndex &idx, DataStore &store)
{
    idx.count = store.count;

    if (idx.count == 0)
    {
        idx.records = nullptr;
        return;
    }

    // Cấp phát mảng con trỏ phẳng có kích thước bằng quy mô DataStore
    idx.records = new LogRecord *[idx.count];

    // Tạo bản sao các con trỏ tham chiếu từ DataStore sang mảng chỉ mục mới
    for (int i = 0; i < idx.count; i++)
    {
        idx.records[i] = get(store, i);
    }

    // Tiến hành sắp xếp mảng con trỏ dựa theo trường dữ liệu timestamp
    if (idx.count > 1)
    {
        mergeSort(idx.records, 0, idx.count - 1);
    }
}

int binarySearchStart(const SortedIndex &idx, long long t)
{
    int left = 0, right = idx.count - 1;
    int result = idx.count; // Trạng thái mặc định nếu toàn bộ bản ghi đều nhỏ hơn t

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (idx.records[mid]->timestamp >= t)
        {
            result = mid;
            right = mid - 1; // Thu hẹp không gian tìm kiếm sang nửa bên trái để tìm vị trí biên đầu tiên
        }
        else
        {
            left = mid + 1;  // Dịch biên sang phân nửa bên phải
        }
    }

    return result;
}

int binarySearchEnd(const SortedIndex &idx, long long t)
{
    int left = 0, right = idx.count - 1;
    int result = -1; // Trạng thái mặc định nếu toàn bộ bản ghi đều lớn hơn t

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (idx.records[mid]->timestamp <= t)
        {
            result = mid;
            left = mid + 1;  // Thu hẹp không gian sang nửa bên phải để tìm vị trí biên cuối cùng
        }
        else
        {
            right = mid - 1; // Dịch biên sang phân nửa bên trái
        }
    }

    return result;
}

void clearSortedIndex(SortedIndex &idx)
{
    if (idx.records != nullptr)
    {
        delete[] idx.records;
        idx.records = nullptr;
    }
    idx.count = 0;
}