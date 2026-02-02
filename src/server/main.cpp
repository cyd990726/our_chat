#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <csignal>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "common/logger.h"
#include "common/config_manager.h"
#include "data/mysql_pool.h"
#include "data/redis_pool.h"

std::unique_ptr<grpc::Server> g_server;

void ShutdownHandler(int) {
    LOG_INFO("Shutting down server...");
    if (g_server) {
        g_server->Shutdown();
    }
}

int main(int argc, char** argv) {
    signal(SIGINT, ShutdownHandler);
    signal(SIGTERM, ShutdownHandler);

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

    std::string server_address = server_config.host + ":" + std::to_string(server_config.port);

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    g_server = builder.BuildAndStart();

    LOG_INFO("Server listening on " + server_address);
    LOG_INFO("OurChat Server started successfully");

    g_server->Wait();

    return 0;
}
