cmake_minimum_required(VERSION 3.16)
project(OurChatServer VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG NO_MODULE)
find_package(yaml-cpp REQUIRED)

set(GRPC_ROOT_DIR "/usr" CACHE PATH "Root directory of gRPC installation")
set(GRPC_INCLUDE_DIR "/usr/include" CACHE PATH "Include directory for gRPC")
set(GRPC_LIBRARY_DIR "/usr/lib/aarch64-linux-gnu" CACHE PATH "Library directory for gRPC")

if(yaml-cpp_FOUND)
    message(STATUS "yaml-cpp found: ${yaml-cpp_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "yaml-cpp not found")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Protobuf_INCLUDE_DIRS}
    ${GRPC_INCLUDE_DIRS}
    ${yaml-cpp_INCLUDE_DIR}
)

link_directories(
    ${Protobuf_LIBRARY_DIRS}
    ${GRPC_LIBRARY_DIRS}
)

add_subdirectory(src/common)
add_subdirectory(src/proto)
add_subdirectory(src/data)
add_subdirectory(src/services)
add_subdirectory(src/server)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
