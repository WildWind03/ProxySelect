//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_DATAURL_H
#define PROXY_DATAURL_H

#include <string>
#include <cstring>
#include <map>

class cache_storage {
    struct cached_data {
        char* data;
        size_t *length;
    };

    std::map<std::string, cached_data> storage;

public:
    void add_info(std::string url, char* data, size_t size1) {
        cached_data cached_data;
        cached_data.data = NULL;
        cached_data.length = NULL;
    }

    void append_info(std::string url, char* data, size_t size) {
        cached_data cached_data1 = storage.find(url).operator*().second;
        if (cached_data1.data == NULL && cached_data1.length == NULL) {
            cached_data1.data = (char*) malloc(size);
            cached_data1.length = (size_t*) malloc(sizeof(size_t));

            strncpy(cached_data1.data, data, size);
            (*cached_data1.length) += size;
            storage.insert(pair<std::string, cached_data> (url, cached_data1));
        }

        (*cached_data1.length) += size;
        cached_data1.data = (char*) realloc(cached_data1.data, size);
        strncat(cached_data1.data, data, size);
    }

    const cached_data get_data_by_url(std::string url) {
        return storage.find(url).operator*().second;
    }

    void delete_by_url(std::string url) {
        cached_data cached_data1 = storage.find(url).operator*().second;
        free(cached_data1.data);
        free(cached_data1.length);
        storage.erase(url);
    }

    virtual ~cache_storage() {
        for (auto iter : storage ) {
            free(iter.second.length);
            free(iter.second.data);
        }
    }
};

#endif //PROXY_DATAURL_H
