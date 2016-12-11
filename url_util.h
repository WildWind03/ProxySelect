//
// Created by alexander on 11.12.16.
//

#ifndef PROXY_URL_UTIL_H
#define PROXY_URL_UTIL_H

#include <string>
#include <algorithm>
#include <fcntl.h>

class url_util {
public:
    url_util() = delete;

    static std::string get_logger_filename_by_url(std::string url) {
        std::string new_logger_name(url);
        std::replace(new_logger_name.begin(), new_logger_name.end(), '.', '_');
        std::replace(new_logger_name.begin(), new_logger_name.end(), '/', '_');
        std::replace(new_logger_name.begin(), new_logger_name.end(), ':', '_');

        return new_logger_name;
    }

    static std::string change_type_of_connection_to_close(std::string request) {
        size_t index = 0;

        while (true) {
            index = request.find("keep-alive", index);
            if (index >= request.length()) {
                break;
            }

            request.replace(index, 10, "Close     ");
            index += 10;
        }

        return request;
    }

    bool static set_socket_blocking_enabled(int fd, bool blocking) {
        if (fd < 0) return false;

        int flags = fcntl(fd, F_GETFL, 0);
        if (flags < 0) return false;
        flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
        return fcntl(fd, F_SETFL, flags) == 0;
    }
};


#endif //PROXY_URL_UTIL_H
