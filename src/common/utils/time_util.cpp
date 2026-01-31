#include "../../../include/common/time_util.h"
#include <sstream>
#include <iomanip>

namespace ourchat {

int64_t TimeUtil::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()
    ).count();
}

int64_t TimeUtil::GetCurrentTimestampMs() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
}

int64_t TimeUtil::GetCurrentTimestampUs() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()
    ).count();
}

std::string TimeUtil::GetCurrentTimeString(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), format.c_str());
    return ss.str();
}

std::string TimeUtil::TimestampToString(int64_t timestamp, const std::string& format) {
    std::stringstream ss;
    ss << std::put_time(std::localtime(reinterpret_cast<const time_t*>(&timestamp)), format.c_str());
    return ss.str();
}

int64_t TimeUtil::StringToTimestamp(const std::string& time_str, const std::string& format) {
    std::tm tm = {};
    std::stringstream ss(time_str);
    ss >> std::get_time(&tm, format.c_str());
    time_t time = std::mktime(&tm);
    return static_cast<int64_t>(time);
}

int64_t TimeUtil::GetTodayStartTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    return static_cast<int64_t>(std::mktime(tm));
}

int TimeUtil::GetDayOfWeek(int64_t timestamp) {
    std::tm* tm;
    if (timestamp == 0) {
        auto now = std::chrono::system_clock::now();
        time_t time = std::chrono::system_clock::to_time_t(now);
        tm = std::localtime(&time);
    } else {
        time_t time = static_cast<time_t>(timestamp);
        tm = std::localtime(&time);
    }
    return tm->tm_wday;
}

int64_t TimeUtil::AddDays(int64_t timestamp, int days) {
    return timestamp + days * 24 * 3600;
}

int64_t TimeUtil::AddHours(int64_t timestamp, int hours) {
    return timestamp + hours * 3600;
}

int64_t TimeUtil::AddMinutes(int64_t timestamp, int minutes) {
    return timestamp + minutes * 60;
}

} // namespace ourchat
