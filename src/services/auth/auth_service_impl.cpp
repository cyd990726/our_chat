#include "services/auth_service_impl.h"
#include "common/logger.h"
#include "common/crypto_util.h"
#include "common/time_util.h"
#include "common/jwt_util.h"
#include "data/mysql_pool.h"
#include "data/redis_pool.h"

namespace ourchat {

AuthServiceImpl::AuthServiceImpl() {
    mysql_pool_ = MySQLPool::Instance();
    redis_pool_ = RedisPool::Instance();
    
    jwt_config_ = ConfigManager::Instance().GetJWTConfig();
}

grpc::Status AuthServiceImpl::Register(grpc::ServerContext* context,
                                       const im::RegisterRequest* request,
                                       im::RegisterResponse* response) {
    LOG_INFO("Register request for user: " + request->username());
    
    auto conn = mysql_pool_->GetConnection();
    if (!conn) {
        response->set_success(false);
        response->set_message("Database connection failed");
        return grpc::Status::OK;
    }
    
    std::string password_hash = CryptoUtil::HashPassword(request->password());
    
    std::string query = "INSERT INTO im_user (username, password_hash, email, create_time, update_time) "
                       "VALUES ('" + request->username() + "', '" + password_hash + "', '" +
                       request->email() + "', " + std::to_string(TimeUtil::GetCurrentTimestamp()) + 
                       ", " + std::to_string(TimeUtil::GetCurrentTimestamp()) + ")";
    
    int64_t insert_id = 0;
    if (!conn->Execute(query, insert_id)) {
        response->set_success(false);
        response->set_message("Username already exists or database error");
        mysql_pool_->ReturnConnection(std::move(conn));
        return grpc::Status::OK;
    }
    
    mysql_pool_->ReturnConnection(std::move(conn));
    
    response->set_success(true);
    response->set_user_id(insert_id);
    
    LOG_INFO("User registered successfully: " + request->username() + ", id: " + std::to_string(insert_id));
    
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::Login(grpc::ServerContext* context,
                                    const im::LoginRequest* request,
                                    im::LoginResponse* response) {
    LOG_INFO("Login request for user: " + request->username());
    
    auto conn = mysql_pool_->GetConnection();
    if (!conn) {
        response->set_success(false);
        response->set_message("Database connection failed");
        return grpc::Status::OK;
    }
    
    std::string query = "SELECT id, password_hash FROM im_user WHERE username = '" + request->username() + "'";
    auto result = conn->Query(query);
    
    if (!result) {
        response->set_success(false);
        response->set_message("User not found");
        mysql_pool_->ReturnConnection(std::move(conn));
        return grpc::Status::OK;
    }
    
    auto row = mysql_fetch_row(result.get());
    if (!row) {
        response->set_success(false);
        response->set_message("Invalid credentials");
        mysql_pool_->ReturnConnection(std::move(conn));
        return grpc::Status::OK;
    }
    
    int64_t user_id = std::stoll(row[0]);
    std::string stored_hash = row[1];
    
    mysql_pool_->ReturnConnection(std::move(conn));
    
    if (!CryptoUtil::ValidatePassword(request->password(), stored_hash)) {
        response->set_success(false);
        response->set_message("Invalid credentials");
        return grpc::Status::OK;
    }
    
    auto token = JWTUtil::GenerateToken(user_id, jwt_config_.secret, jwt_config_.expire_seconds);
    
    auto redis_conn = redis_pool_->GetConnection();
    if (redis_conn) {
        redis_conn->Setex("token:" + token, jwt_config_.expire_seconds, 
                         std::to_string(user_id));
        redis_conn->Setex("user_token:" + std::to_string(user_id), 
                         jwt_config_.expire_seconds, token);
        redis_pool_->ReturnConnection(std::move(redis_conn));
    }
    
    response->set_success(true);
    response->set_user_id(user_id);
    response->set_token(token);
    
    LOG_INFO("User logged in successfully: " + request->username());
    
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::Logout(grpc::ServerContext* context,
                                     const im::LogoutRequest* request,
                                     im::LogoutResponse* response) {
    LOG_INFO("Logout request for user: " + std::to_string(request->user_id()));
    
    auto redis_conn = redis_pool_->GetConnection();
    if (redis_conn) {
        redis_conn->Del("user_token:" + std::to_string(request->user_id()));
        redis_pool_->ReturnConnection(std::move(redis_conn));
    }
    
    response->set_success(true);
    
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::RefreshToken(grpc::ServerContext* context,
                                           const im::RefreshTokenRequest* request,
                                           im::RefreshTokenResponse* response) {
    LOG_INFO("Token refresh request");
    
    int64_t user_id = 0;
    if (!JWTUtil::ValidateToken(request->token(), user_id, jwt_config_.secret)) {
        response->set_success(false);
        response->set_message("Invalid token");
        return grpc::Status::OK;
    }
    
    auto new_token = JWTUtil::GenerateToken(user_id, jwt_config_.secret, jwt_config_.expire_seconds);
    
    auto redis_conn = redis_pool_->GetConnection();
    if (redis_conn) {
        redis_conn->Setex("token:" + new_token, jwt_config_.expire_seconds, 
                         std::to_string(user_id));
        redis_conn->Setex("user_token:" + std::to_string(user_id), 
                         jwt_config_.expire_seconds, new_token);
        redis_pool_->ReturnConnection(std::move(redis_conn));
    }
    
    response->set_success(true);
    response->set_token(new_token);
    
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::ValidateToken(grpc::ServerContext* context,
                                            const im::ValidateTokenRequest* request,
                                            im::ValidateTokenResponse* response) {
    int64_t user_id = 0;
    if (JWTUtil::ValidateToken(request->token(), user_id, jwt_config_.secret)) {
        response->set_success(true);
        response->set_user_id(user_id);
    } else {
        response->set_success(false);
    }
    
    return grpc::Status::OK;
}

} // namespace ourchat
