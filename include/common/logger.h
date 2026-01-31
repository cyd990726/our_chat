#ifndef OURCHAT_LOGGER_H
#define OURCHAT_LOGGER_H

#include <string>
#include <memory>
#include <functional>
#include <mutex>

namespace ourchat {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
public:
    Logger();
    ~Logger();
    
    static std::shared_ptr<Logger> GetInstance();
    
    void SetLogLevel(LogLevel level);
    void SetOutputFile(const std::string& filepath);
    void SetConsoleOutput(bool enable);
    
    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warn(const std::string& message);
    void Error(const std::string& message);
    void Fatal(const std::string& message);
    
    void Log(LogLevel level, const std::string& message);
    
    static std::string LevelToString(LogLevel level);
    static LogLevel StringToLevel(const std::string& level);
    
private:
    void Write(LogLevel level, const std::string& message);
    std::string GetCurrentTime();
    
    LogLevel current_level_;
    std::string output_file_;
    bool console_output_;
    std::mutex mutex_;
    FILE* file_handle_;
};

#define LOG_DEBUG(msg) ::ourchat::Logger::GetInstance()->Debug(msg)
#define LOG_INFO(msg) ::ourchat::Logger::GetInstance()->Info(msg)
#define LOG_WARN(msg) ::ourchat::Logger::GetInstance()->Warn(msg)
#define LOG_ERROR(msg) ::ourchat::Logger::GetInstance()->Error(msg)
#define LOG_FATAL(msg) ::ourchat::Logger::GetInstance()->Fatal(msg)

} // namespace ourchat

#endif // OURCHAT_LOGGER_H
