//
// Created by alexander on 07.12.16.
//

#ifndef PROXY_DATAURL_H
#define PROXY_DATAURL_H

#include <string>
#include <cstring>
#include <map>
#include "request_base.h"
#include <vector>

class cache_storage {
    struct cached_data;

    std::multimap<std::string, cached_data*> storage;

public:
    const static int CACHE_RECORD_MAX_CAPACITY = 4096;

    struct cached_data {
        char *data;
        size_t length;
        bool is_finished;
        bool is_streaming;

        std::vector<request_base> clients;
    };

    void add_info(std::string url) {
        cached_data *cached_data1 = new cached_data();
        cached_data1 -> length = 0;
        cached_data1 -> data = (char*) malloc(sizeof(CACHE_RECORD_MAX_CAPACITY));

        storage.insert(std::pair<std::string, cached_data*> (url, cached_data1));
    }

    /*void append_info(std::string url, char* data, size_t size) {
        cached_data *cached_data1 = storage.find(url).operator*().second;

        if (0 == cached_data1 -> length) {
            cached_data1 -> is_streaming = false;
            cached_data1 -> is_finished = false;
            cached_data1 -> data = (char*) malloc(size);

            strncpy(cached_data1 -> data, data, size);
            cached_data1 -> length += size;
        } else {
            cached_data1 -> length += size;
            cached_data1 -> data = (char*) realloc(cached_data1 -> data, size);
            strncat(cached_data1 -> data, data, size);
        }
    }*/

    void finish_record(std::string url) {
        storage.find(url).operator*().second -> is_finished = true;
    }

    cached_data* get_not_streaming_data_by_url(std::string url) {
        auto iter = storage.find(url);
        while (iter != storage.end()) {
            if (storage.find(url).operator*().second -> is_streaming == false) {
                return iter.operator*().second;
            }
        }

        return NULL;
    }

    void delete_by_url(std::string url) {
        cached_data *cached_data1 = storage.find(url).operator*().second;
        free(cached_data1 -> data);
        delete(cached_data1);
        storage.erase(url);
    }

    virtual ~cache_storage() {
        for (auto iter : storage ) {
            free(iter.second -> data);
            delete(iter.second);
        }
    }
};

#endif //PROXY_DATAURL_H
