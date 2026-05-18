/**
 * @file        SessionBuilder.cpp
 * @brief       Implementation giải thuật gom cụm phiên dựa trên thời gian timeout và trạng thái định danh log.
 */

#include "session/SessionBuilder.h"
#include "anomaly/AnomalyConfig.h"

// ================================================================================
//  Helper functions (Nội bộ)
// ================================================================================

/**
 * @brief Thêm một con trỏ phiên hoạt động mới vào danh sách quản lý phiên, tự động mở rộng nếu đầy.
 * @param list Tham chiếu tới danh sách quản lý phiên hoạt động tổng thể
 * @param session Con trỏ thực thể phiên hoạt động cần bổ sung
 */
static void pushSession(SessionList &list, UserSession *session)
{
    if (list.count == list.capacity)
    {
        int newCapacity = list.capacity == 0 ? 1000 : list.capacity * 2;
        UserSession **newSessions = new UserSession *[newCapacity];
        
        for (int i = 0; i < list.count; i++)
        {
            newSessions[i] = list.sessions[i];
        }
        
        if (list.sessions != nullptr)
        {
            delete[] list.sessions;
        }
        
        list.sessions = newSessions;
        list.capacity = newCapacity;
    }
    
    list.sessions[list.count++] = session;
}

// ================================================================================
//  Public functions
// ================================================================================

void buildSessions(SessionList &sessionList, const HashIndex &hashIdx)
{
    sessionList.count = 0;
    sessionList.capacity = 1000;
    sessionList.sessions = new UserSession *[sessionList.capacity];

    // Duyệt qua từng bucket trong bảng băm của User ID để xử lý cục bộ cho từng người dùng
    for (int bucket = 0; bucket < hashIdx.byUser.tableSize; bucket++)
    {
        HashNode *node = hashIdx.byUser.buckets[bucket];
        
        while (node != nullptr)
        {
            int count = node->count;
            if (count == 0)
            {
                node = node->next;
                continue;
            }

            // Các sự kiện vốn dĩ đã được đảm bảo sắp xếp theo thời gian tăng dần nhờ tầng Deduplicator trước đó
            UserSession *currentSession = nullptr;

            for (int i = 0; i < count; i++)
            {
                LogRecord *event = node->values[i];
                bool shouldStartNew = false;

                // Xác định ranh giới để khởi tạo một phiên hoạt động mới
                if (currentSession == nullptr)
                {
                    shouldStartNew = true;
                }
                else if (event->event_type == EventType::LOGIN)
                {
                    shouldStartNew = true; // Sự kiện LOGIN chủ động bắt buộc mở một phiên mới
                }
                else
                {
                    // Kiểm tra khoảng trống thời gian giữa sự kiện hiện tại và sự kiện liền trước có vượt ngưỡng không
                    long long lastEventTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                    if (event->timestamp - lastEventTime > (AnomalyConfig::SESSION_TIMEOUT_MINS * 60))
                    {
                        shouldStartNew = true; // Vượt quá thời gian cấu hình tối đa -> Hết hạn phiên cũ
                    }
                }

                if (shouldStartNew)
                {
                    // Đóng gói và lưu lại phiên cũ trước khi sang phiên mới
                    if (currentSession != nullptr)
                    {
                        currentSession->endTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                        pushSession(sessionList, currentSession);
                    }

                    // Khởi tạo các thông số trạng thái ban đầu cho một phiên mới hoàn toàn
                    currentSession = new UserSession();
                    currentSession->userId = node->key;
                    currentSession->startTime = event->timestamp;
                    currentSession->eventCapacity = 10;
                    currentSession->eventCount = 0;
                    currentSession->events = new LogRecord *[currentSession->eventCapacity];
                }

                // Đưa con trỏ sự kiện hiện tại vào danh sách mảng động nội bộ của phiên
                if (currentSession->eventCount == currentSession->eventCapacity)
                {
                    int newCap = currentSession->eventCapacity * 2;
                    LogRecord **newArr = new LogRecord *[newCap];
                    
                    for (int j = 0; j < currentSession->eventCount; ++j)
                    {
                        newArr[j] = currentSession->events[j];
                    }
                    
                    delete[] currentSession->events;
                    currentSession->events = newArr;
                    currentSession->eventCapacity = newCap;
                }
                
                currentSession->events[currentSession->eventCount++] = event;

                // Điều kiện đặc biệt: Đóng phiên ngay lập tức nếu bắt gặp tín hiệu chủ động LOGOUT
                if (event->event_type == EventType::LOGOUT)
                {
                    currentSession->endTime = event->timestamp;
                    pushSession(sessionList, currentSession);
                    currentSession = nullptr; // Reset con trỏ để vòng lặp kế tiếp tự động tạo phiên mới
                }
            }

            // Đóng gói phiên hoạt động mở cuối cùng còn sót lại của người dùng này (nếu có)
            if (currentSession != nullptr)
            {
                currentSession->endTime = currentSession->events[currentSession->eventCount - 1]->timestamp;
                pushSession(sessionList, currentSession);
            }

            node = node->next;
        }
    }
}

void clearSessions(SessionList &sessionList)
{
    if (sessionList.sessions == nullptr)
        return;

    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        if (session != nullptr)
        {
            if (session->events != nullptr)
            {
                delete[] session->events; // Giải phóng mảng con trỏ chứa sự kiện nội bộ
            }
            delete session; // Giải phóng cấu trúc đối tượng phiên
        }
    }

    delete[] sessionList.sessions;
    sessionList.sessions = nullptr;
    sessionList.count = 0;
    sessionList.capacity = 0;
}