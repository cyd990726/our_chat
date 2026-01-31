#pragma once
#include <string>
#include <vector>
#include "common.pb.h"

namespace im {

class Message {
public:
    int64_t message_id() const { return message_id_; }
    void set_message_id(int64_t value) { message_id_ = value; }
    int64_t sender_id() const { return sender_id_; }
    void set_sender_id(int64_t value) { sender_id_ = value; }
    int64_t receiver_id() const { return receiver_id_; }
    void set_receiver_id(int64_t value) { receiver_id_ = value; }
    int message_type() const { return message_type_; }
    void set_message_type(int value) { message_type_ = value; }
    const std::string& content() const { return content_; }
    void set_content(const std::string& value) { content_ = value; }
    int64_t timestamp() const { return timestamp_; }
    void set_timestamp(int64_t value) { timestamp_ = value; }
    
    int64_t message_id_ = 0;
    int64_t sender_id_ = 0;
    int64_t receiver_id_ = 0;
    int message_type_ = 0;
    std::string content_;
    int64_t timestamp_ = 0;
};

class SendMessageRequest {
public:
    int64_t sender_id() const { return sender_id_; }
    void set_sender_id(int64_t value) { sender_id_ = value; }
    int64_t receiver_id() const { return receiver_id_; }
    void set_receiver_id(int64_t value) { receiver_id_ = value; }
    int message_type() const { return message_type_; }
    void set_message_type(int value) { message_type_ = value; }
    const std::string& content() const { return content_; }
    void set_content(const std::string& value) { content_ = value; }
    
    int64_t sender_id_ = 0;
    int64_t receiver_id_ = 0;
    int message_type_ = 0;
    std::string content_;
};

class SendMessageResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    int64_t message_id() const { return message_id_; }
    void set_message_id(int64_t value) { message_id_ = value; }
    const std::string& message() const { return message_; }
    void set_message(const std::string& value) { message_ = value; }
    
    bool success_ = false;
    int64_t message_id_ = 0;
    std::string message_;
};

class GetMessagesRequest {
public:
    int64_t user_id() const { return user_id_; }
    void set_user_id(int64_t value) { user_id_ = value; }
    int64_t peer_id() const { return peer_id_; }
    void set_peer_id(int64_t value) { peer_id_ = value; }
    int64_t timestamp() const { return timestamp_; }
    void set_timestamp(int64_t value) { timestamp_ = value; }
    int32_t limit() const { return limit_; }
    void set_limit(int32_t value) { limit_ = value; }
    
    int64_t user_id_ = 0;
    int64_t peer_id_ = 0;
    int64_t timestamp_ = 0;
    int32_t limit_ = 20;
};

class GetMessagesResponse {
public:
    bool success() const { return success_; }
    void set_success(bool value) { success_ = value; }
    RepeatedPtrField<Message>* mutable_messages() { return &messages_; }
    const RepeatedPtrField<Message>& messages() const { return messages_; }
    
    bool success_ = false;
    RepeatedPtrField<Message> messages_;
};

} // namespace im
