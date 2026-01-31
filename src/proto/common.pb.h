#pragma once
#include <string>
#include <vector>

namespace im {

template<typename T>
class RepeatedPtrField {
public:
    std::vector<T>& mutable_vector() { return items_; }
    const std::vector<T>& items() const { return items_; }
    void Add() { items_.emplace_back(); }
    void Add(const T& item) { items_.push_back(item); }
    size_t size() const { return items_.size(); }
    
private:
    std::vector<T> items_;
};

class Empty {};

class Error {
public:
    int32_t code() const { return code_; }
    void set_code(int32_t value) { code_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    int32_t code_ = 0;
    std::string message_;
};

class MessageType {
public:
    static constexpr int TEXT = 0;
    static constexpr int IMAGE = 1;
    static constexpr int VOICE = 2;
    static constexpr int VIDEO = 3;
    static constexpr int FILE = 4;
    static constexpr int LOCATION = 5;
};

class SessionType {
public:
    static constexpr int UNSPECIFIED = 0;
    static constexpr int SINGLE = 1;
    static constexpr int GROUP = 2;
};

class MemberRole {
public:
    static constexpr int UNSPECIFIED = 0;
    static constexpr int MEMBER = 1;
    static constexpr int ADMIN = 2;
    static constexpr int OWNER = 3;
};

class OnlineStatus {
public:
    static constexpr int OFFLINE = 0;
    static constexpr int ONLINE = 1;
    static constexpr int AWAY = 2;
};

} // namespace im
