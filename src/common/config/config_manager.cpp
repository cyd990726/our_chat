#include "../../../include/common/config_manager.h"
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>

namespace ourchat {

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::LoadConfig(const std::string& config_path) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        
        if (!config["mysql"]) {
            std::cerr << "MySQL config not found" << std::endl;
            return false;
        }
        
        mysql_.host = config["mysql"]["host"].as<std::string>();
        mysql_.port = config["mysql"]["port"].as<int>();
        mysql_.username = config["mysql"]["username"].as<std::string>();
        mysql_.password = config["mysql"]["password"].as<std::string>();
        mysql_.database = config["mysql"]["database"].as<std::string>();
        mysql_.pool_size = config["mysql"]["pool_size"].as<int>(10);
        mysql_.max_pool_size = config["mysql"]["max_pool_size"].as<int>(20);
        mysql_.connection_timeout = config["mysql"]["connection_timeout"].as<int>(10);
        mysql_.idle_timeout = config["mysql"]["idle_timeout"].as<int>(300);
        
        if (!config["redis"]) {
            std::cerr << "Redis config not found" << std::endl;
            return false;
        }
        
        redis_.host = config["redis"]["host"].as<std::string>();
        redis_.port = config["redis"]["port"].as<int>();
        redis_.password = config["redis"]["password"].as<std::string>("");
        redis_.db = config["redis"]["db"].as<int>(0);
        redis_.pool_size = config["redis"]["pool_size"].as<int>(10);
        redis_.command_timeout = config["redis"]["command_timeout"].as<int>(5);
        
        if (config["kafka"]) {
            auto brokers_node = config["kafka"]["brokers"];
            for (const auto& broker : brokers_node) {
                kafka_.brokers.push_back(broker.as<std::string>());
            }
            kafka_.port = config["kafka"]["port"].as<int>(9092);
            kafka_.client_id = config["kafka"]["client_id"].as<std::string>("im_server");
            kafka_.batch_size = config["kafka"]["batch_size"].as<int>(16384);
            kafka_.linger_ms = config["kafka"]["linger_ms"].as<int>(5);
            kafka_.compression = config["kafka"]["compression"].as<int>(1);
            kafka_.acks = config["kafka"]["acks"].as<int>(-1);
            kafka_.retries = config["kafka"]["retries"].as<int>(3);
        }
        
        if (config["server"]) {
            server_.service_name = config["server"]["service_name"].as<std::string>("ourchat_server");
            server_.host = config["server"]["host"].as<std::string>("0.0.0.0");
            server_.port = config["server"]["port"].as<int>(50051);
            server_.worker_threads = config["server"]["worker_threads"].as<int>(4);
            server_.max_connection = config["server"]["max_connection"].as<int>(10000);
            server_.keepalive_time = config["server"]["keepalive_time"].as<int>(30);
            server_.keepalive_timeout = config["server"]["keepalive_timeout"].as<int>(10);
        }
        
        if (config["jwt"]) {
            jwt_.secret = config["jwt"]["secret"].as<std::string>();
            jwt_.expire_seconds = config["jwt"]["expire_seconds"].as<int>(86400);
            jwt_.refresh_expire_seconds = config["jwt"]["refresh_expire_seconds"].as<int>(604800);
        }
        
        return true;
    } catch (const YAML::Exception& e) {
        std::cerr << "Failed to parse config file: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load config: " << e.what() << std::endl;
        return false;
    }
}

const DatabaseConfig& ConfigManager::GetDatabaseConfig() const {
    return mysql_;
}

const RedisConfig& ConfigManager::GetRedisConfig() const {
    return redis_;
}

const KafkaConfig& ConfigManager::GetKafkaConfig() const {
    return kafka_;
}

const ServerConfig& ConfigManager::GetServerConfig() const {
    return server_;
}

const JWTConfig& ConfigManager::GetJWTConfig() const {
    return jwt_;
}

} // namespace ourchat
