add_executable(ourchat_server
    main.cpp
)

target_link_libraries(ourchat_server PUBLIC
    common
    data
    services
    proto
    grpc++
    protobuf::libprotobuf
    yaml-cpp
)

install(TARGETS ourchat_server
    RUNTIME DESTINATION bin
)
