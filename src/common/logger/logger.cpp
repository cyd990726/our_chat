#include "../../../include/common/logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <cstring>
#include <iostream>

namespace ourchat {

std::shared_ptr<Logger> Logger::GetInstance() {
    static std::shared_ptr<Logger> instance = std::make_shared<Logger>();
    return instance;
}

Logger::Logger() : current_level_(LogLevel::INFO), 
                   console_output_(true), 
                   file_handle_(nullptr) {
}

Logger::~Logger() {
    if (file_handle_) {
        fclose(file_handle_);
    }
}

void Logger::SetLogLevel(LogLevel level) {
    current_level_ = level;
}

void Logger::SetOutputFile(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (file_handle_) {
        fclose(file_handle_);
    }
    
    output_file_ = filepath;
    file_handle_ = fopen(filepath.c_str(), "a");
    
    if (!file_handle_) {
        std::cerr << "Failed to open log file: " << filepath << std::endl;
    }
}

void Logger::SetConsoleOutput(bool enable) {
    console_output_ = enable;
}

void Logger::Debug(const std::string& message) {
    Log(LogLevel::DEBUG, message);
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::INFO, message);
}

void Logger::Warn(const std::string& message) {
    Log(LogLevel::WARN, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::ERROR, message);
}

void Logger::Fatal(const std::string& message) {
    Log(LogLevel::FATAL, message);
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < current_level_) {
        return;
    }
    
    Write(level, message);
}

void Logger::Write(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::string log_message = "[" + GetCurrentTime() + "] [" + 
                              LevelToString(level) + "] " + message + "\n";
    
    if (console_output_) {
        std::cout << log_message;
        std::cout.flush();
    }
    
    if (file_handle_) {
        fwrite(log_message.c_str(), 1, log_message.length(), file_handle_);
        fflush(file_handle_);
    }
}

std::string Logger::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

LogLevel Logger::StringToLevel(const std::string& level) {
    if (level == "DEBUG") return LogLevel::DEBUG;
    if (level == "INFO") return LogLevel::INFO;
    if (level == "WARN") return LogLevel::WARN;
    if (level == "ERROR") return LogLevel::ERROR;
    if (level == "FATAL") return LogLevel::FATAL;
    return LogLevel::INFO;
}

} // namespace ourchat
