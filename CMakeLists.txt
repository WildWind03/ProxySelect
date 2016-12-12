cmake_minimum_required(VERSION 3.5)
project(Proxy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp proxy_server.h proxy_server.cpp exception_proxy_not_created.h http_request_parser.h exception_too_much_data.h request_server.h request_base.h request_client.h exception_connection_closed_while_sending_request.h exception_read.h exception_not_supported_request.h exception_invalid_http_data.h request_enum.h exception_can_not_connect.h exception_write.h http_response_parser.h cached_data.h observable.h observer.h event_type.h logger.h url_util.h exception_connection_closed_while_receiving_response.h)
#set(HTTP_PARSER_LIB http-parser-master/http_request_parser.c http-parser-master/http_request_parser.h)

#add_library(http_lib STATIC ${HTTP_PARSER_LIB})
add_executable(Proxy ${SOURCE_FILES})

#target_link_libraries(Proxy http_lib)