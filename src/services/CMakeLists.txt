add_library(services
    auth/auth_service_impl.cpp
    message/message_service_impl.cpp
    group/group_service_impl.cpp
    session/session_service_impl.cpp
    presence/presence_service_impl.cpp
)

target_link_libraries(services PUBLIC
    common
    data
    proto
    grpc++
)
