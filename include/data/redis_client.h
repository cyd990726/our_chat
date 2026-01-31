#ifndef OURCHAT_REDIS_CLIENT_H
#define OURCHAT_REDIS_CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <hiredis/hiredis.h>

namespace ourchat {

class RedisClient {
public:
    RedisClient();
    ~RedisClient();
    
    bool Connect(const std::string& host, int port, const std::string& password = "", int db = 0);
    void Close();
    
    bool IsConnected();
    
    bool Set(const std::string& key, const std::string& value);
    bool Setex(const std::string& key, int seconds, const std::string& value);
    bool Setnx(const std::string& key, const std::string& value);
    
    std::string Get(const std::string& key);
    bool Del(const std::string& key);
    bool Del(const std::vector<std::string>& keys);
    bool Exists(const std::string& key);
    
    bool HSet(const std::string& key, const std::string& field, const std::string& value);
    bool HDel(const std::string& key, const std::string& field);
    std::string HGet(const std::string& key, const std::string& field);
    std::map<std::string, std::string> HGetAll(const std::string& key);
    std::vector<std::string> HMGet(const std::string& key, const std::vector<std::string>& fields);
    
    bool SAdd(const std::string& key, const std::string& member);
    bool SRem(const std::string& key, const std::string& member);
    std::vector<std::string> SMembers(const std::string& key);
    bool SIsMember(const std::string& key, const std::string& member);
    int SCard(const std::string& key);
    
    bool ZAdd(const std::string& key, double score, const std::string& member);
    bool ZRem(const std::string& key, const std::string& member);
    std::vector<std::string> ZRange(const std::string& key, int start, int stop);
    std::vector<std::string> ZRevRange(const std::string& key, int start, int stop);
    std::vector<std::string> ZRangeByScore(const std::string& key, double min, double max);
    std::vector<std::string> ZRevRangeByScore(const std::string& key, double min, double max);
    int ZCard(const std::string& key);
    
    int64_t Incr(const std::string& key);
    int64_t IncrBy(const std::string& key, int64_t increment);
    int64_t Decr(const std::string& key);
    
    bool Expire(const std::string& key, int seconds);
    int64_t TTL(const std::string& key);
    
    redisContext* GetRawContext();
    
private:
    redisContext* context_;
    bool connected_;
};

} // namespace ourchat

#endif // OURCHAT_REDIS_CLIENT_H
