file(GLOB PROTO_SRCS "*.pb.cc")
file(GLOB PROTO_HDRS "*.pb.h" "*.grpc.pb.h")

add_library(proto ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(proto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(proto PUBLIC
    protobuf::libprotobuf
    grpc++
)
