//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_HTTP_RESPONSE_PARSER_H
#define PROXY_HTTP_RESPONSE_PARSER_H
class http_response_parser {

public:
    http_response_parser(const char * response) {

    }

    bool is_ok_response() {
        return true;
    }

    int get_major_version() {
        return 1;
    }

    int get_minor_version() {
        return 0;
    }
};
#endif //PROXY_HTTP_RESPONSE_PARSER_H
