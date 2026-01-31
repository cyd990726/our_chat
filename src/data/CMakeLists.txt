add_library(data
    mysql/mysql_connection.cpp
    mysql/mysql_pool.cpp
    redis/redis_client.cpp
    redis/redis_pool.cpp
)

target_link_libraries(data PUBLIC
    common
    protobuf::libprotobuf
    hiredis
   mariadb
)
