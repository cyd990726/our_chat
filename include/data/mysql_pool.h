#ifndef OURCHAT_MYSQL_POOL_H
#define OURCHAT_MYSQL_POOL_H

#include "mysql_connection.h"
#include "../common/config.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <atomic>

namespace ourchat {

class MySQLPool {
public:
    static std::shared_ptr<MySQLPool> Instance();
    
    bool Init(const DatabaseConfig& config);
    std::unique_ptr<MySQLConnection> GetConnection();
    void ReturnConnection(std::unique_ptr<MySQLConnection> connection);
    
    void Close();
    
    int GetPoolSize();
    int GetActiveConnections();
    int GetIdleConnections();
    
public:
    ~MySQLPool();

private:
    MySQLPool() = default;
    
    void CreateConnection();
    void CheckConnections();
    
    std::queue<std::unique_ptr<MySQLConnection>> connections_;
    std::mutex mutex_;
    std::condition_variable cv_;
    
    DatabaseConfig config_;
    std::atomic<bool> running_{false};
    std::thread monitor_thread_;
    
    int active_count_ = 0;
};

} // namespace ourchat

#endif // OURCHAT_MYSQL_POOL_H
