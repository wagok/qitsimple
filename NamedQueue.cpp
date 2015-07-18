//
// Created by root on 7/17/15.
//

#include "NamedQueue.h"

void NamedQueue::push(std::string name, int priority, std::string *data) {

    _lock.lock();
    auto search = queue_map.find(name);
    if(search != queue_map.end()) {
        SimpleQueue *tmp = search->second;
        _lock.unlock();
        tmp->push(priority, data);
    }
    else {
        auto queue = new SimpleQueue();
        queue->push(priority, data);
        queue_map[name] = queue;
    }
    _lock.unlock();
}

std::string *NamedQueue::pop(std::string name) {

    _lock.lock();
    auto search = queue_map.find(name);
    if(search != queue_map.end()) {
        SimpleQueue *tmp = search->second;
        _lock.unlock();
        std::string * data =  tmp->pop();
        if (tmp->length() == 0) {
            queue_map.erase(search);
            delete tmp;
        }
        return data;
    }
    _lock.unlock();
    return NULL;
}

unsigned long NamedQueue::length(std::string name) {

    _lock.lock();
    auto search = queue_map.find(name);
    if(search != queue_map.end()) {
        SimpleQueue *tmp = search->second;
        _lock.unlock();
        return tmp->length();
    }
    _lock.unlock();
    return 0;
}
