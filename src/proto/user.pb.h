#pragma once
#include <string>
#include <vector>
#include "common.pb.h"

namespace im {

class RegisterRequest {
public:
    const std::string& username() const { return username_; }
    void set_username(const std::string& value) { username_ = value; }
    const std::string& password() const { return password_; }
    void set_password(const std::string& value) { password_ = value; }
    const std::string& email() const { return email_; }
    void set_email(const std::string& value) { email_ = value; }
    
    std::string username_;
    std::string password_;
    std::string email_;
};

class RegisterResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    
    bool success_ = false;
    std::string message_;
    int64_t user_id_ = 0;
};

class LoginRequest {
public:
    const std::string& username() const { return username_; }
    void set_username(const std::string& value) { username_ = value; }
    const std::string& password() const { return password_; }
    void set_password(const std::string& value) { password_ = value; }
    
    std::string username_;
    std::string password_;
};

class LoginResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    const std::string& token() const { return token_; }
    void set_token(const std::string& value) { token_ = value; }
    
    bool success_ = false;
    std::string message_;
    int64_t user_id_ = 0;
    std::string token_;
};

class LogoutRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    const std::string& token() const { return token_; }
    void set_token(const std::string& value) { token_ = value; }
    
    int64_t user_id_ = 0;
    std::string token_;
};

class LogoutResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    bool success_ = false;
    std::string message_;
};

class RefreshTokenRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    const std::string& token() const { return token_; }
    void set_token(const std::string& value) { token_ = value; }
    
    int64_t user_id_ = 0;
    std::string token_;
};

class RefreshTokenResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    const std::string& token() const { return token_; }
    void set_token(const std::string& value) { token_ = value; }
    
    bool success_ = false;
    std::string message_;
    std::string token_;
};

class ValidateTokenRequest {
public:
    const std::string& token() const { return token_; }
    void set_token(const std::string& value) { token_ = value; }
    
    std::string token_;
};

class ValidateTokenResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    
    bool success_ = false;
    int64_t user_id_ = 0;
};

} // namespace im
