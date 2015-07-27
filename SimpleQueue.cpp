//
// Created by root on 7/16/15.
//

#include "SimpleQueue.h"
#include "Logger.h"

std::string* SimpleQueue::pop() {

    _lock.lock();
    std::string *str;
    if(queue.empty()) {
       str = NULL;
    } else {
        auto elem = queue.top();
        if (elem->priority == 0 && elem->timestamp >= std::time(nullptr)) {
            str = NULL;
        } else {
            str = elem->data;
            queue.pop();
            delete elem;
        }
    }
    _lock.unlock();
    return str;
}

void SimpleQueue::push(int priority, std::string* data) {
    QueueElem *elem = new QueueElem(priority, 0, data);
    _lock.lock();
    queue.push(elem);
    _lock.unlock();
}

unsigned long SimpleQueue::length() {
    _lock.lock();
    unsigned long size = queue.size();
    _lock.unlock();
    return size;
}

void SimpleQueue::clear() {
    _lock.lock();
    while(!queue.empty()) {
        auto elem = queue.top();
        delete(elem->data);
        queue.pop();
        delete elem;
    }
    _lock.unlock();
}

void SimpleQueue::push_postponed(int timestamp, std::string *data) {
    timestamp += std::time(nullptr);
    QueueElem *elem = new QueueElem(0, timestamp, data);
    _lock.lock();
    queue.push(elem);
    _lock.unlock();
}
