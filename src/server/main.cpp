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
    builder.SetDefaultCompressionAlgorithm(GRPC_COMPRESS_GZIP);
    
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

int main(int argc, char** argv) {
    std::string config_path = "config/server.yaml";
    
    if (argc > 1) {
        config_path = argv[1];
    }
    
    if (!ourchat::ConfigManager::Instance().LoadConfig(config_path)) {
        std::cerr << "Failed to load configuration" << std::endl;
        return 1;
    }
    
    auto& config = ourchat::ConfigManager::Instance();
    
    LOG_INFO("Initializing OurChat Server...");
    
    auto mysql_config = config.GetDatabaseConfig();
    if (!ourchat::MySQLPool::Instance()->Init(mysql_config)) {
        LOG_ERROR("Failed to initialize MySQL pool");
        return 1;
    }
    
    auto redis_config = config.GetRedisConfig();
    if (!ourchat::RedisPool::Instance()->Init(redis_config)) {
        LOG_ERROR("Failed to initialize Redis pool");
        return 1;
    }
    
    auto server_config = config.GetServerConfig();
    LOG_INFO("Server configuration loaded: " + server_config.service_name);
    
    RunServer();
    
    return 0;
}
