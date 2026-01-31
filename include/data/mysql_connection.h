#ifndef OURCHAT_MYSQL_CONNECTION_H
#define OURCHAT_MYSQL_CONNECTION_H

#include <string>
#include <memory>
#include <mysql/mysql.h>

namespace ourchat {

class MySQLConnection {
public:
    MySQLConnection();
    ~MySQLConnection();
    
    bool Connect(const std::string& host, int port, 
                 const std::string& username, const std::string& password,
                 const std::string& database);
    void Close();
    
    bool Execute(const std::string& query);
    bool Execute(const std::string& query, int64_t& insert_id);
    
    std::unique_ptr<MYSQL_RES> Query(const std::string& query);
    
    bool BeginTransaction();
    bool Commit();
    bool Rollback();
    
    bool Ping();
    
    MYSQL* GetRawConnection();
    
private:
    MYSQL* connection_;
    bool connected_;
};

} // namespace ourchat

#endif // OURCHAT_MYSQL_CONNECTION_H
