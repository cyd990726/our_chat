#pragma once
#include <string>
#include <vector>
#include "common.pb.h"

namespace im {

class GetFriendsRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    
    int64_t user_id_ = 0;
};

class FriendInfo {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    const std::string& username() const { return username_; }
    void set_username(const std::string& value) { username_ = value; }
    const std::string& avatar_url() const { return avatar_url_; }
    void set_avatar_url(const std::string& value) { avatar_url_ = value; }
    int online_status() const { return online_status_; }
    void set_online_status(int value) { online_status_ = value; }
    
    int64_t user_id_ = 0;
    std::string username_;
    std::string avatar_url_;
    int online_status_ = 0;
};

class GetFriendsResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    RepeatedPtrField<FriendInfo>* mutable_friends() { return &friends_; }
    const RepeatedPtrField<FriendInfo>& friends() const { return friends_; }
    
    bool success_ = false;
    RepeatedPtrField<FriendInfo> friends_;
};

class AddFriendRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    int64_t friend_id() const { return friend_id_; }
    void set_friend_id(int64_t value) { friend_id_ = value; }
    
    int64_t user_id_ = 0;
    int64_t friend_id_ = 0;
};

class AddFriendResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    bool success_ = false;
    std::string message_;
};

} // namespace im
