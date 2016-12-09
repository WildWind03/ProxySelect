//
// Created by alexander on 09.12.16.
//

#ifndef PROXY_OBSERVABLE_H
#define PROXY_OBSERVABLE_H

#include <map>
#include "observer.h"

class observable {

    std::map<int, observer*> observers;

public:
    void notify(event_type event_type1) {
        for (auto & iter : observers) {
            iter.second -> update(event_type1);
        }
    }

    void notify(int fd, event_type event_type1) {
        observers.find(fd).operator*().second->update(event_type1);
    }

    virtual void add_new_observer(observer *observer1, int key) {
        observers.insert(std::pair<int, observer*> (key, observer1));
    }

    void delete_observer(int key) {
        observers.erase(key);
    }
};

#endif //PROXY_OBSERVABLE_H
