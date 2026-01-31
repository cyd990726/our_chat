#include "../../../include/data/mysql_connection.h"
#include "../../../include/common/logger.h"

namespace ourchat {

MySQLConnection::MySQLConnection() : connection_(nullptr), connected_(false) {
    connection_ = mysql_init(nullptr);
    if (!connection_) {
        LOG_ERROR("Failed to initialize MySQL connection");
    }
}

MySQLConnection::~MySQLConnection() {
    Close();
}

bool MySQLConnection::Connect(const std::string& host, int port,
                              const std::string& username, const std::string& password,
                              const std::string& database) {
    if (!connection_) {
        connection_ = mysql_init(nullptr);
        if (!connection_) return false;
    }
    
    mysql_options(connection_, MYSQL_SET_CHARSET_NAME, "utf8mb4");
    
    if (!mysql_real_connect(connection_, host.c_str(), username.c_str(), 
                           password.c_str(), database.c_str(), port, nullptr, 0)) {
        LOG_ERROR("Failed to connect to MySQL: " + std::string(mysql_error(connection_)));
        return false;
    }
    
    connected_ = true;
    LOG_INFO("Successfully connected to MySQL database");
    return true;
}

void MySQLConnection::Close() {
    if (connection_) {
        mysql_close(connection_);
        connection_ = nullptr;
        connected_ = false;
    }
}

bool MySQLConnection::Execute(const std::string& query) {
    if (!connected_) return false;
    
    if (mysql_query(connection_, query.c_str())) {
        LOG_ERROR("MySQL query failed: " + std::string(mysql_error(connection_)));
        return false;
    }
    return true;
}

bool MySQLConnection::Execute(const std::string& query, int64_t& insert_id) {
    if (!Execute(query)) return false;
    
    insert_id = mysql_insert_id(connection_);
    return true;
}

std::unique_ptr<MYSQL_RES> MySQLConnection::Query(const std::string& query) {
    if (!connected_) return nullptr;
    
    if (mysql_query(connection_, query.c_str())) {
        LOG_ERROR("MySQL query failed: " + std::string(mysql_error(connection_)));
        return nullptr;
    }
    
    MYSQL_RES* result = mysql_store_result(connection_);
    if (!result) {
        LOG_ERROR("MySQL store result failed: " + std::string(mysql_error(connection_)));
        return nullptr;
    }
    
    return std::unique_ptr<MYSQL_RES>(result);
}

bool MySQLConnection::BeginTransaction() {
    return Execute("START TRANSACTION");
}

bool MySQLConnection::Commit() {
    return Execute("COMMIT");
}

bool MySQLConnection::Rollback() {
    return Execute("ROLLBACK");
}

bool MySQLConnection::Ping() {
    if (!connected_) return false;
    return mysql_ping(connection_) == 0;
}

MYSQL* MySQLConnection::GetRawConnection() {
    return connection_;
}

} // namespace ourchat
