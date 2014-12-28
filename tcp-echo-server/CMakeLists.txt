cmake_minimum_required(VERSION 2.8.4)

project(tcp_echo_server)

set(SOURCE_FILES main.c)

include_directories("/usr/local/include" "${PROJECT_SOURCE_DIR}/deps")

link_directories("/usr/local/lib")

add_executable(tcp_echo_server ${SOURCE_FILES})

target_link_libraries(tcp_echo_server "libuv.a")
