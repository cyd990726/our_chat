#include "../../../include/data/redis_pool.h"
#include "../../../include/common/logger.h"

namespace ourchat {

std::shared_ptr<RedisPool> RedisPool::Instance() {
    static std::shared_ptr<RedisPool> instance = std::make_shared<RedisPool>();
    return instance;
}

bool RedisPool::Init(const RedisConfig& config) {
    config_ = config;
    running_ = true;
    
    for (int i = 0; i < config.pool_size; i++) {
        CreateConnection();
    }
    
    LOG_INFO("Redis pool initialized with " + std::to_string(config_.pool_size) + " connections");
    return true;
}

RedisPool::~RedisPool() {
    Close();
}

std::unique_ptr<RedisClient> RedisPool::GetConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    
    cv_.wait(lock, [this]() {
        return !connections_.empty() || !running_;
    });
    
    if (!running_) return nullptr;
    
    auto connection = std::move(connections_.front());
    connections_.pop();
    active_count_++;
    
    return connection;
}

void RedisPool::ReturnConnection(std::unique_ptr<RedisClient> connection) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (connection->IsConnected()) {
        connections_.push(std::move(connection));
    }
    
    active_count_--;
    cv_.notify_one();
}

void RedisPool::Close() {
    running_ = false;
    cv_.notify_all();
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    while (!connections_.empty()) {
        connections_.pop();
    }
}

void RedisPool::CreateConnection() {
    auto connection = std::make_unique<RedisClient>();
    
    if (connection->Connect(config_.host, config_.port, 
                           config_.password, config_.db)) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.push(std::move(connection));
    } else {
        LOG_ERROR("Failed to create Redis connection");
    }
}

int RedisPool::GetPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size() + active_count_;
}

int RedisPool::GetActiveConnections() {
    return active_count_;
}

int RedisPool::GetIdleConnections() {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size();
}

} // namespace ourchat
