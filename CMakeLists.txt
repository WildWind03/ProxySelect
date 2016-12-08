cmake_minimum_required(VERSION 3.5)
project(Proxy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp proxy_server.h proxy_server.cpp proxy_not_created_exception.h client.h http_parser.h too_much_data_exception.h server_request.h cache_storage.h base_request.h client_request.h connection_close_exception.h exec_request_exception.h request_not_supported_exception.h http_parse_exception.h request_enum.h)
#set(HTTP_PARSER_LIB http-parser-master/http_parser.c http-parser-master/http_parser.h)

#add_library(http_lib STATIC ${HTTP_PARSER_LIB})
add_executable(Proxy ${SOURCE_FILES})

#target_link_libraries(Proxy http_lib)