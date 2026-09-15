cmake_minimum_required(VERSION 3.28)
project(firewatch C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED IMPORTED_TARGET libwebsockets)

add_executable(firewatch
        main.c
        websocket_server.c
        server_log.c
)

target_link_libraries(firewatch PRIVATE PkgConfig::LWS)