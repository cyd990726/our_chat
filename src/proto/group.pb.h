#pragma once
#include <string>
#include <vector>
#include "common.pb.h"

namespace im {

class CreateGroupRequest {
public:
    const std::string& group_name() const { return group_name_; }
    void set_group_name(const std::string& value) { group_name_ = value; }
    int64_t owner_id() const { return owner_id_; }
    void set_owner_id(int64_t value) { owner_id_ = value; }
    const std::string& description() const { return description_; }
    void set_description(const std::string& value) { description_ = value; }
    
    std::string group_name_;
    int64_t owner_id_ = 0;
    std::string description_;
};

class CreateGroupResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    int64_t group_id() const { return group_id_; }
    void set_group_id(int64_t value) { group_id_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    bool success_ = false;
    int64_t group_id_ = 0;
    std::string message_;
};

class GetGroupInfoRequest {
public:
    int64_t group_id() const { return group_id_; }
    void set_group_id(int64_t value) { group_id_ = value; }
    
    int64_t group_id_ = 0;
};

class GroupInfo {
public:
    int64_t group_id() const { return group_id_; }
    void set_group_id(int64_t value) { group_id_ = value; }
    const std::string& group_name() const { return group_name_; }
    void set_group_name(const std::string& value) { group_name_ = value; }
    const std::string& description() const { return description_; }
    void set_description(const std::string& value) { description_ = value; }
    int64_t owner_id() const { return owner_id_; }
    void set_owner_id(int64_t value) { owner_id_ = value; }
    int32_t member_count() const { return member_count_; }
    void set_member_count(int32_t value) { member_count_ = value; }
    
    int64_t group_id_ = 0;
    std::string group_name_;
    std::string description_;
    int64_t owner_id_ = 0;
    int32_t member_count_ = 0;
};

class GetGroupInfoResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    GroupInfo* mutable_group_info() { return &group_info_; }
    const GroupInfo& group_info() const { return group_info_; }
    
    bool success_ = false;
    GroupInfo group_info_;
};

} // namespace im
