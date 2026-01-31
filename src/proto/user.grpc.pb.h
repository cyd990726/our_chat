#pragma once
#include "user.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class UserService {
public:
    virtual ~UserService() = default;

    virtual grpc::Status Register(grpc::ServerContext* context,
                                  const RegisterRequest* request,
                                  RegisterResponse* response) = 0;

    virtual grpc::Status Login(grpc::ServerContext* context,
                               const LoginRequest* request,
                               LoginResponse* response) = 0;

    virtual grpc::Status Logout(grpc::ServerContext* context,
                                const LogoutRequest* request,
                                LogoutResponse* response) = 0;

    virtual grpc::Status RefreshToken(grpc::ServerContext* context,
                                      const RefreshTokenRequest* request,
                                      RefreshTokenResponse* response) = 0;

    virtual grpc::Status ValidateToken(grpc::ServerContext* context,
                                       const ValidateTokenRequest* request,
                                       ValidateTokenResponse* response) = 0;
};

typedef UserService AuthService;

} // namespace im
