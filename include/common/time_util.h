#ifndef OURCHAT_TIME_UTIL_H
#define OURCHAT_TIME_UTIL_H

#include <string>
#include <chrono>
#include <ctime>

namespace ourchat {

class TimeUtil {
public:
    static int64_t GetCurrentTimestamp();
    static int64_t GetCurrentTimestampMs();
    static int64_t GetCurrentTimestampUs();
    
    static std::string GetCurrentTimeString(const std::string& format = "%Y-%m-%d %H:%M:%S");
    static std::string TimestampToString(int64_t timestamp, const std::string& format = "%Y-%m-%d %H:%M:%S");
    static int64_t StringToTimestamp(const std::string& time_str, const std::string& format = "%Y-%m-%d %H:%M:%S");
    
    static int64_t GetTodayStartTimestamp();
    static int GetDayOfWeek(int64_t timestamp = 0);
    
    static int64_t AddDays(int64_t timestamp, int days);
    static int64_t AddHours(int64_t timestamp, int hours);
    static int64_t AddMinutes(int64_t timestamp, int minutes);
};

} // namespace ourchat

#endif // OURCHAT_TIME_UTIL_H
