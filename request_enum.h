//
// Created by alexander on 08.12.16.
//

#ifndef PROXY_REQUEST_ENUM_H
#define PROXY_REQUEST_ENUM_H


enum class request_enum {
    READ, READ_FROM_CLIENT_FINISHED, WRITE, WRITE_TO_SERVER_FINISHED, WRITE_TO_CLIENT_FINISHED, READ_FROM_SERVER_FINISHED
};


#endif //PROXY_REQUEST_ENUM_H
