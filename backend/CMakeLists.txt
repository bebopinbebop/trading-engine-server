cmake_minimum_required(VERSION 3.28)
project(trading_engine_server C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LWS REQUIRED IMPORTED_TARGET libwebsockets)

add_executable(trading_engine_server main.c)

target_link_libraries(trading_engine_server PRIVATE PkgConfig::LWS)