#include "../../../include/data/redis_client.h"
#include "../../../include/common/logger.h"
#include <sstream>
#include <cstring>

namespace ourchat {

RedisClient::RedisClient() : context_(nullptr), connected_(false) {
}

RedisClient::~RedisClient() {
    Close();
}

bool RedisClient::Connect(const std::string& host, int port, const std::string& password, int db) {
    struct timeval timeout = {5, 0};
    
    context_ = redisConnectWithTimeout(host.c_str(), port, timeout);
    if (!context_ || context_->err) {
        if (context_) {
            LOG_ERROR("Redis connection error: " + std::string(context_->errstr));
            redisFree(context_);
        } else {
            LOG_ERROR("Redis connection failed: allocation failed");
        }
        return false;
    }
    
    if (!password.empty()) {
        auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                          "AUTH %s", password.c_str()));
        if (reply) {
            freeReplyObject(reply);
        }
    }
    
    if (db != 0) {
        auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                          "SELECT %d", db));
        if (reply) {
            freeReplyObject(reply);
        }
    }
    
    connected_ = true;
    LOG_INFO("Connected to Redis at " + host + ":" + std::to_string(port));
    return true;
}

void RedisClient::Close() {
    if (context_) {
        redisFree(context_);
        context_ = nullptr;
        connected_ = false;
    }
}

bool RedisClient::IsConnected() {
    return connected_ && context_ && context_->err == 0;
}

bool RedisClient::Set(const std::string& key, const std::string& value) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SET %s %s", key.c_str(), value.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_STATUS && 
                   strcmp(reply->str, "OK") == 0);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Setex(const std::string& key, int seconds, const std::string& value) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SETEX %s %d %s", 
                                                      key.c_str(), seconds, value.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_STATUS && 
                   strcmp(reply->str, "OK") == 0);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Setnx(const std::string& key, const std::string& value) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SETNX %s %s", key.c_str(), value.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);
    return success;
}

std::string RedisClient::Get(const std::string& key) {
    if (!IsConnected()) return "";
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "GET %s", key.c_str()));
    if (!reply) return "";
    
    std::string result;
    if (reply->type == REDIS_REPLY_STRING) {
        result = reply->str;
    }
    freeReplyObject(reply);
    return result;
}

bool RedisClient::Del(const std::string& key) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "DEL %s", key.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Del(const std::vector<std::string>& keys) {
    if (!IsConnected()) return false;
    
    std::stringstream ss;
    ss << "DEL";
    for (const auto& key : keys) {
        ss << " " << key;
    }
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, ss.str().c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::Exists(const std::string& key) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "EXISTS %s", key.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::HSet(const std::string& key, const std::string& field, const std::string& value) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "HSET %s %s %s", 
                                                      key.c_str(), field.c_str(), value.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::HDel(const std::string& key, const std::string& field) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "HDEL %s %s", key.c_str(), field.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

std::string RedisClient::HGet(const std::string& key, const std::string& field) {
    if (!IsConnected()) return "";
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "HGET %s %s", key.c_str(), field.c_str()));
    if (!reply) return "";
    
    std::string result;
    if (reply->type == REDIS_REPLY_STRING) {
        result = reply->str;
    }
    freeReplyObject(reply);
    return result;
}

std::map<std::string, std::string> RedisClient::HGetAll(const std::string& key) {
    std::map<std::string, std::string> result;
    
    if (!IsConnected()) return result;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "HGETALL %s", key.c_str()));
    if (!reply) return result;
    
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i += 2) {
            if (i + 1 < reply->elements) {
                result[reply->element[i]->str] = reply->element[i + 1]->str;
            }
        }
    }
    
    freeReplyObject(reply);
    return result;
}

std::vector<std::string> RedisClient::SMembers(const std::string& key) {
    std::vector<std::string> result;
    
    if (!IsConnected()) return result;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SMEMBERS %s", key.c_str()));
    if (!reply) return result;
    
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i++) {
            result.push_back(reply->element[i]->str);
        }
    }
    
    freeReplyObject(reply);
    return result;
}

bool RedisClient::SIsMember(const std::string& key, const std::string& member) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SISMEMBER %s %s", 
                                                      key.c_str(), member.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::SAdd(const std::string& key, const std::string& member) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SADD %s %s", key.c_str(), member.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::SRem(const std::string& key, const std::string& member) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SREM %s %s", key.c_str(), member.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

int RedisClient::SCard(const std::string& key) {
    if (!IsConnected()) return 0;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "SCARD %s", key.c_str()));
    if (!reply) return 0;
    
    int count = (reply->type == REDIS_REPLY_INTEGER) ? reply->integer : 0;
    freeReplyObject(reply);
    return count;
}

bool RedisClient::ZAdd(const std::string& key, double score, const std::string& member) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "ZADD %s %f %s", 
                                                      key.c_str(), score, member.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

bool RedisClient::ZRem(const std::string& key, const std::string& member) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "ZREM %s %s", key.c_str(), member.c_str()));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER);
    freeReplyObject(reply);
    return success;
}

std::vector<std::string> RedisClient::ZRange(const std::string& key, int start, int stop) {
    std::vector<std::string> result;
    
    if (!IsConnected()) return result;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "ZRANGE %s %d %d", 
                                                      key.c_str(), start, stop));
    if (!reply) return result;
    
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i++) {
            result.push_back(reply->element[i]->str);
        }
    }
    
    freeReplyObject(reply);
    return result;
}

std::vector<std::string> RedisClient::ZRevRange(const std::string& key, int start, int stop) {
    std::vector<std::string> result;
    
    if (!IsConnected()) return result;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "ZREVRANGE %s %d %d", 
                                                      key.c_str(), start, stop));
    if (!reply) return result;
    
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i++) {
            result.push_back(reply->element[i]->str);
        }
    }
    
    freeReplyObject(reply);
    return result;
}

int64_t RedisClient::Incr(const std::string& key) {
    if (!IsConnected()) return 0;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "INCR %s", key.c_str()));
    if (!reply) return 0;
    
    int64_t value = (reply->type == REDIS_REPLY_INTEGER) ? reply->integer : 0;
    freeReplyObject(reply);
    return value;
}

int64_t RedisClient::IncrBy(const std::string& key, int64_t increment) {
    if (!IsConnected()) return 0;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "INCRBY %s %lld", 
                                                      key.c_str(), increment));
    if (!reply) return 0;
    
    int64_t value = (reply->type == REDIS_REPLY_INTEGER) ? reply->integer : 0;
    freeReplyObject(reply);
    return value;
}

int64_t RedisClient::Decr(const std::string& key) {
    if (!IsConnected()) return 0;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "DECR %s", key.c_str()));
    if (!reply) return 0;
    
    int64_t value = (reply->type == REDIS_REPLY_INTEGER) ? reply->integer : 0;
    freeReplyObject(reply);
    return value;
}

bool RedisClient::Expire(const std::string& key, int seconds) {
    if (!IsConnected()) return false;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "EXPIRE %s %d", key.c_str(), seconds));
    if (!reply) return false;
    
    bool success = (reply->type == REDIS_REPLY_INTEGER && reply->integer == 1);
    freeReplyObject(reply);
    return success;
}

int64_t RedisClient::TTL(const std::string& key) {
    if (!IsConnected()) return -1;
    
    auto reply = static_cast<redisReply*>(redisCommand(context_, 
                                                      "TTL %s", key.c_str()));
    if (!reply) return -1;
    
    int64_t ttl = (reply->type == REDIS_REPLY_INTEGER) ? reply->integer : -1;
    freeReplyObject(reply);
    return ttl;
}

redisContext* RedisClient::GetRawContext() {
    return context_;
}

} // namespace ourchat
