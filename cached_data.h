//
// Created by alexander on 09.12.16.
//

#ifndef PROXY_CACHED_DATA_H
#define PROXY_CACHED_DATA_H


#include <cstddef>
#include <vector>
#include "observable.h"

class cached_data : public observable {

    char *data;
    size_t length = 0;
    size_t min_pos_in_cache_record = 0;

    std::map<int, size_t> pos_in_cache;

    observer *server_observer;

public:
    const static size_t MAX_CAPACITY_OF_CACHE_RECORD = 128000;

    bool is_finished = false;
    bool is_streaming = false;

    virtual void add_new_observer(observer *observer1, int key) override {
        observable::add_new_observer(observer1, key);
        pos_in_cache.insert(std::pair<int, int> (key, 0));
    }

    cached_data() {
        data = (char*) malloc (MAX_CAPACITY_OF_CACHE_RECORD);
    }

    char* get_data_to_write() {
        return data + length;
    }

    size_t get_max_data_can_be_written() {
        return MAX_CAPACITY_OF_CACHE_RECORD - length;
    }

    size_t get_length() {
        return length;
    }

    void add_server_observer(observer *observer1) {
        this -> server_observer = observer1;
    }

    bool update_because_data_was_read(int fd, size_t count_of_read_bytes) {
        size_t pos = pos_in_cache.find(fd).operator*().second;
        pos += count_of_read_bytes;

        if (is_streaming) {
            size_t min = 0;
            for (auto &iter : pos_in_cache) {
                size_t possible_new_min = iter.second;
                if (possible_new_min < min) {
                    min = possible_new_min;
                }
            }

            if (min < min_pos_in_cache_record) {
                min_pos_in_cache_record = min;
            }

            if (min_pos_in_cache_record == MAX_CAPACITY_OF_CACHE_RECORD) {
                min_pos_in_cache_record = 0;
                length = 0;
                notify(event_type::DISABLE_READ);
                server_observer->update(event_type::ENABLE_WRITE);
            }
        }

        if (is_finished) {
            return pos == length;
        } else {
            if (pos == length) {
                notify(fd, event_type::DISABLE_READ);
            }

            return false;
        }
    }

    void update_because_data_was_add(size_t count_of_add_data) {
        this -> length += count_of_add_data;

        if (this -> length >= MAX_CAPACITY_OF_CACHE_RECORD && !is_streaming) {
            this -> is_streaming = true;
        }

        if (this -> length >= MAX_CAPACITY_OF_CACHE_RECORD) {
            server_observer -> update(event_type::DISABLE_WRITE);
        }

        notify(event_type::ENABLE_READ);
    }

    char* get_data_to_read(int fd) {
        return data + pos_in_cache.find(fd).operator*().second;
    }

    size_t get_count_of_bytes_that_can_be_read(int fd) {
        return length - pos_in_cache.find(fd).operator*().second;
    }

    void mark_finished() {
        is_finished = true;
    }


    virtual ~cached_data() {
        free (data);
    }
};


#endif //PROXY_CACHED_DATA_H
