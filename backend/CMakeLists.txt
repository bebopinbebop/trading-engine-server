cmake_minimum_required(VERSION 3.28)
project(firewatch C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED IMPORTED_TARGET libwebsockets)

add_executable(firewatch
        main.c
        logging/server_log.c
        server/websocket_server.c
)

target_include_directories(firewatch PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
