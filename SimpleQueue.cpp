//
// Created by root on 7/16/15.
//

#include "SimpleQueue.h"

std::string* SimpleQueue::pop() {

    _lock.lock();
    std::string *str;
    if(queue.empty()) {
       str = NULL;
    } else {
        auto elem = queue.top();
        str = elem->data;
        queue.pop();
        delete elem;
    }
    _lock.unlock();
    return str;
}

void SimpleQueue::push(int priority, std::string* data) {
    QueueElem *elem = new QueueElem(priority, data);
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
