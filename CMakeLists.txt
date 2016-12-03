cmake_minimum_required(VERSION 3.5)
project(Proxy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp proxy_server.h proxy_server.cpp proxy_not_created_exception.h inet_socket_address.h)
set(HTTP_PARSER_LIB http-parser-master/http_parser.c http-parser-master/http_parser.h)

add_library(http_lib STATIC ${HTTP_PARSER_LIB})
add_executable(Proxy ${SOURCE_FILES})

target_link_libraries(Proxy http_lib)