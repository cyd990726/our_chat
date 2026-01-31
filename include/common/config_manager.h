#ifndef OURCHAT_CONFIG_MANAGER_H
#define OURCHAT_CONFIG_MANAGER_H

#include "config.h"
#include <string>

namespace ourchat {

class ConfigManager {
public:
    static ConfigManager& Instance();
    
    bool LoadConfig(const std::string& config_path);
    
    const DatabaseConfig& GetDatabaseConfig() const;
    const RedisConfig& GetRedisConfig() const;
    const KafkaConfig& GetKafkaConfig() const;
    const ServerConfig& GetServerConfig() const;
    const JWTConfig& GetJWTConfig() const;
    
private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    DatabaseConfig mysql_;
    RedisConfig redis_;
    KafkaConfig kafka_;
    ServerConfig server_;
    JWTConfig jwt_;
};

} // namespace ourchat

#endif // OURCHAT_CONFIG_MANAGER_H
