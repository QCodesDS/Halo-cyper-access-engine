/**
 * @file        SessionDetector.cpp
 * @brief       Implementation các thuật toán phân tích bất thường trên thực thể phiên (Session).
 */

#include "anomaly/SessionDetector.h"
#include "anomaly/AnomalyConfig.h"

// ================================================================================
//  Public functions
// ================================================================================

void detectLongSession(AnomalyList &results, const SessionList &sessionList)
{
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        if (session->endTime - session->startTime > (AnomalyConfig::MAX_SESSION_HOURS * 3600))
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::LONG_SESSION;
            rec->userId = session->userId;
            rec->timestamp = session->startTime;
            rec->detail = "Session lasted " + std::to_string((session->endTime - session->startTime) / 3600) + " hours";
            pushAnomaly(results, rec);
        }
    }
}

void detectSessionFlood(AnomalyList &results, const SessionList &sessionList)
{
    // Do SessionBuilder gom nhóm tuần tự theo từng user, các phiên của cùng một 
    // tài khoản sẽ nằm liên tiếp nhau trong mảng phẳng sessionList.
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *currentSession = sessionList.sessions[i];
        int sessionCount = 0;

        // Quét ngược lại các phiên TRƯỚC ĐÓ của CÙNG user nằm trong khung thời gian 60 phút
        for (int j = i; j >= 0; j--)
        {
            UserSession *pastSession = sessionList.sessions[j];
            if (pastSession->userId != currentSession->userId)
            {
                break; // Đã chuyển sang vùng dữ liệu của user khác
            }

            if (currentSession->startTime - pastSession->startTime > 3600)
            {
                break; // Vượt quá giới hạn khung thời gian 1 giờ
            }

            sessionCount++;
        }

        if (sessionCount > AnomalyConfig::MAX_SESSIONS_PER_HOUR)
        {
            AnomalyRecord *rec = new AnomalyRecord();
            rec->type = AnomalyType::SESSION_FLOOD;
            rec->userId = currentSession->userId;
            rec->timestamp = currentSession->startTime;
            rec->detail = std::to_string(sessionCount) + " sessions started in 60 min";
            pushAnomaly(results, rec);
        }
    }
}

void detectDangerousSequence(AnomalyList &results, const SessionList &sessionList)
{
    for (int i = 0; i < sessionList.count; i++)
    {
        UserSession *session = sessionList.sessions[i];
        
        for (int j = 0; j < session->eventCount; j++)
        {
            LogRecord *event = session->events[j];
            
            if (event->event_type == EventType::ADMIN_ACTION)
            {
                // Quét tuyến tính các sự kiện kế tiếp xem có hành vi DOWNLOAD trong 10 phút hay không
                for (int k = j + 1; k < session->eventCount; k++)
                {
                    LogRecord *futureEvent = session->events[k];
                    
                    if (futureEvent->timestamp - event->timestamp > (AnomalyConfig::DANGEROUS_SEQ_MINS * 60))
                    {
                        break; // Đã vượt khung thời gian 10 phút an toàn
                    }
                    
                    if (futureEvent->event_type == EventType::DOWNLOAD)
                    {
                        AnomalyRecord *rec = new AnomalyRecord();
                        rec->type = AnomalyType::DANGEROUS_SEQUENCE;
                        rec->userId = session->userId;
                        rec->timestamp = futureEvent->timestamp;
                        rec->detail = "ADMIN_ACTION -> DOWNLOAD in " + std::to_string((futureEvent->timestamp - event->timestamp) / 60) + " min";
                        pushAnomaly(results, rec);
                        break; // Chỉ cần cảnh báo một lần cho chuỗi hành vi này của phiên
                    }
                }
            }
        }
    }
}