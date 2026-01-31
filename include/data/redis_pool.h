#ifndef OURCHAT_REDIS_POOL_H
#define OURCHAT_REDIS_POOL_H

#include "redis_client.h"
#include "common/config.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <atomic>

namespace ourchat {

class RedisPool {
public:
    static std::shared_ptr<RedisPool> Instance();
    
    bool Init(const RedisConfig& config);
    std::unique_ptr<RedisClient> GetConnection();
    void ReturnConnection(std::unique_ptr<RedisClient> connection);
    
    void Close();
    
    int GetPoolSize();
    int GetActiveConnections();
    int GetIdleConnections();
    
public:
    RedisPool() = default;
    ~RedisPool();
    
    void CreateConnection();
    
    std::queue<std::unique_ptr<RedisClient>> connections_;
    std::mutex mutex_;
    std::condition_variable cv_;
    
    RedisConfig config_;
    std::atomic<bool> running_{false};
    
    int active_count_ = 0;
};

} // namespace ourchat

#endif // OURCHAT_REDIS_POOL_H
