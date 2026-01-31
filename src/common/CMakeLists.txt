add_library(common
    config/config_manager.cpp
    logger/logger.cpp
    utils/time_util.cpp
    utils/string_util.cpp
    utils/crypto_util.cpp
)

target_link_libraries(common PUBLIC
    yaml-cpp
    pthread
    OpenSSL::Crypto
    uuid
)
