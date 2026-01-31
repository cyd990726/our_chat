#pragma once
#include <string>
#include <vector>
#include "common.pb.h"

namespace im {

class SetOnlineRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    int online_status() const { return online_status_; }
    void set_online_status(int value) { online_status_ = value; }
    
    int64_t user_id_ = 0;
    int online_status_ = 0;
};

class SetOnlineResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    bool success_ = false;
    std::string message_;
};

class GetOnlineStatusRequest {
public:
    RepeatedPtrField<int64_t>* mutable_user_ids() { return &user_ids_; }
    const RepeatedPtrField<int64_t>& user_ids() const { return user_ids_; }
    
    RepeatedPtrField<int64_t> user_ids_;
};

class UserStatus {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    int online_status() const { return online_status_; }
    void set_online_status(int value) { online_status_ = value; }
    
    int64_t user_id_ = 0;
    int online_status_ = 0;
};

class GetOnlineStatusResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    RepeatedPtrField<UserStatus>* mutable_statuses() { return &statuses_; }
    const RepeatedPtrField<UserStatus>& statuses() const { return statuses_; }
    
    bool success_ = false;
    RepeatedPtrField<UserStatus> statuses_;
};

} // namespace im
