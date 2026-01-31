#ifndef OURCHAT_CONFIG_H
#define OURCHAT_CONFIG_H

#include <string>
#include <vector>
#include <memory>

namespace ourchat {

struct DatabaseConfig {
    std::string host;
    int port;
    std::string username;
    std::string password;
    std::string database;
    int pool_size;
    int max_pool_size;
    int connection_timeout;
    int idle_timeout;
};

struct RedisConfig {
    std::string host;
    int port;
    std::string password;
    int db;
    int pool_size;
    int command_timeout;
};

struct KafkaConfig {
    std::vector<std::string> brokers;
    int port;
    std::string client_id;
    int batch_size;
    int linger_ms;
    int compression;
    int acks;
    int retries;
};

struct ServerConfig {
    std::string service_name;
    std::string host;
    int port;
    int worker_threads;
    int max_connection;
    int keepalive_time;
    int keepalive_timeout;
};

struct JWTConfig {
    std::string secret;
    int expire_seconds;
    int refresh_expire_seconds;
};

struct Config {
    DatabaseConfig mysql;
    RedisConfig redis;
    KafkaConfig kafka;
    ServerConfig server;
    JWTConfig jwt;
};

} // namespace ourchat

#endif // OURCHAT_CONFIG_H
