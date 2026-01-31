#include "../../../include/data/mysql_pool.h"
#include "../../../include/common/logger.h"

namespace ourchat {

std::shared_ptr<MySQLPool> MySQLPool::Instance() {
    static std::shared_ptr<MySQLPool> instance(new MySQLPool());
    return instance;
}

bool MySQLPool::Init(const DatabaseConfig& config) {
    config_ = config;
    running_ = true;
    
    for (int i = 0; i < config.pool_size; i++) {
        CreateConnection();
    }
    
    monitor_thread_ = std::thread([this]() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::seconds(30));
            CheckConnections();
        }
    });
    
    LOG_INFO("MySQL pool initialized with " + std::to_string(config_.pool_size) + " connections");
    return true;
}

MySQLPool::~MySQLPool() {
    Close();
}

std::unique_ptr<MySQLConnection> MySQLPool::GetConnection() {
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

void MySQLPool::ReturnConnection(std::unique_ptr<MySQLConnection> connection) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (connection->Ping()) {
        connections_.push(std::move(connection));
    }
    
    active_count_--;
    cv_.notify_one();
}

void MySQLPool::Close() {
    running_ = false;
    cv_.notify_all();
    
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    while (!connections_.empty()) {
        connections_.pop();
    }
}

void MySQLPool::CreateConnection() {
    auto connection = std::make_unique<MySQLConnection>();
    
    if (connection->Connect(config_.host, config_.port,
                           config_.username, config_.password,
                           config_.database)) {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.push(std::move(connection));
    } else {
        LOG_ERROR("Failed to create MySQL connection");
    }
}

void MySQLPool::CheckConnections() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::queue<std::unique_ptr<MySQLConnection>> new_connections;
    
    while (!connections_.empty()) {
        auto connection = std::move(connections_.front());
        connections_.pop();
        
        if (connection->Ping()) {
            new_connections.push(std::move(connection));
        } else {
            LOG_WARN("Reconnecting to MySQL");
            auto new_conn = std::make_unique<MySQLConnection>();
            if (new_conn->Connect(config_.host, config_.port,
                                 config_.username, config_.password,
                                 config_.database)) {
                new_connections.push(std::move(new_conn));
            }
        }
    }
    
    connections_ = std::move(new_connections);
}

int MySQLPool::GetPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size() + active_count_;
}

int MySQLPool::GetActiveConnections() {
    return active_count_;
}

int MySQLPool::GetIdleConnections() {
    std::lock_guard<std::mutex> lock(mutex_);
    return connections_.size();
}

} // namespace ourchat
