#ifndef OURCHAT_AUTH_SERVICE_IMPL_H
#define OURCHAT_AUTH_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include "user.grpc.pb.h"
#include "data/mysql_pool.h"
#include "data/redis_pool.h"
#include "common/config_manager.h"

namespace ourchat {

class AuthServiceImpl : public im::AuthService, public grpc::Service {
public:
    AuthServiceImpl();
    
    grpc::Status Register(grpc::ServerContext* context,
                         const im::RegisterRequest* request,
                         im::RegisterResponse* response) override;
    
    grpc::Status Login(grpc::ServerContext* context,
                       const im::LoginRequest* request,
                       im::LoginResponse* response) override;
    
    grpc::Status Logout(grpc::ServerContext* context,
                        const im::LogoutRequest* request,
                        im::LogoutResponse* response) override;
    
    grpc::Status RefreshToken(grpc::ServerContext* context,
                              const im::RefreshTokenRequest* request,
                              im::RefreshTokenResponse* response) override;
    
    grpc::Status ValidateToken(grpc::ServerContext* context,
                               const im::ValidateTokenRequest* request,
                               im::ValidateTokenResponse* response) override;
    
private:
    std::shared_ptr<MySQLPool> mysql_pool_;
    std::shared_ptr<RedisPool> redis_pool_;
    JWTConfig jwt_config_;
};

} // namespace ourchat

#endif // OURCHAT_AUTH_SERVICE_IMPL_H
