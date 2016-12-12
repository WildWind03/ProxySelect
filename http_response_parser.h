//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_HTTP_RESPONSE_PARSER_H
#define PROXY_HTTP_RESPONSE_PARSER_H

#include <sstream>
#include <iostream>

class http_response_parser {
    const int SUCCESS_CODE = 200;
    int code;

public:
    http_response_parser(const char * response) {
        std::stringstream stringstream(response);

        std::string protocol_version;
        stringstream >> protocol_version;
        std::string code_request;
        stringstream >> code_request;

        code = std::stoi(code_request);
    }

    bool is_ok_response() {
        return code == SUCCESS_CODE;
    }

};
#endif //PROXY_HTTP_RESPONSE_PARSER_H
