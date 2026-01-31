#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "common/logger.h"
#include "common/config_manager.h"
#include "data/mysql_pool.h"
#include "data/redis_pool.h"
#include "services/auth_service_impl.h"
#include "services/message_service_impl.h"
#include "services/group_service_impl.h"
#include "services/session_service_impl.h"
#include "services/presence_service_impl.h"

void RunServer() {
    auto& config = ourchat::ConfigManager::Instance();
    auto& server_config = config.GetServerConfig();
    
    std::string server_address = server_config.host + ":" + std::to_string(server_config.port);
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.SetMaxReceiveMessageSize(10 * 1024 * 1024);
    builder.SetMaxSendMessageSize(10 * 1024 * 1024);
    
    std::unique_ptr<ourchat::AuthServiceImpl> auth_service(new ourchat::AuthServiceImpl());
    std::unique_ptr<ourchat::MessageServiceImpl> message_service(new ourchat::MessageServiceImpl());
    std::unique_ptr<ourchat::GroupServiceImpl> group_service(new ourchat::GroupServiceImpl());
    std::unique_ptr<ourchat::SessionServiceImpl> session_service(new ourchat::SessionServiceImpl());
    std::unique_ptr<ourchat::PresenceServiceImpl> presence_service(new ourchat::PresenceServiceImpl());
    
    builder.RegisterService(auth_service.get());
    builder.RegisterService(message_service.get());
    builder.RegisterService(group_service.get());
    builder.RegisterService(session_service.get());
    builder.RegisterService(presence_service.get());
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    LOG_INFO("Server listening on " + server_address);
    LOG_INFO("OurChat Server started successfully");
    
    server->Wait();
}

void RunServerInternal() {
    auto& config = ourchat::ConfigManager::Instance();
    auto& server_config = config.GetServerConfig();
    
    std::string server_address = server_config.host + ":" + std::to_string(server_config.port);
    
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.SetMaxReceiveMessageSize(10 * 1024 * 1024);
    builder.SetMaxSendMessageSize(10 * 1024 * 1024);
    
    std::unique_ptr<ourchat::AuthServiceImpl> auth_service(new ourchat::AuthServiceImpl());
    std::unique_ptr<ourchat::MessageServiceImpl> message_service(new ourchat::MessageServiceImpl());
    std::unique_ptr<ourchat::GroupServiceImpl> group_service(new ourchat::GroupServiceImpl());
    std::unique_ptr<ourchat::SessionServiceImpl> session_service(new ourchat::SessionServiceImpl());
    std::unique_ptr<ourchat::PresenceServiceImpl> presence_service(new ourchat::PresenceServiceImpl());
    
    builder.RegisterService(auth_service.get());
    builder.RegisterService(message_service.get());
    builder.RegisterService(group_service.get());
    builder.RegisterService(session_service.get());
    builder.RegisterService(presence_service.get());
    
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    LOG_INFO("Server listening on " + server_address);
    LOG_INFO("OurChat Server started successfully");
    
    server->Wait();
}
