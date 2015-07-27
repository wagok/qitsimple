//
// Created by root on 7/17/15.
//

#ifndef LINUX_DEAMON_NAMEDQUEUE_H
#define LINUX_DEAMON_NAMEDQUEUE_H

#include <string>
#include <unordered_map>
#include <mutex>
#include "SimpleQueue.h"

class NamedQueue {
private:
    std::mutex _lock;
public:
    std::unordered_map<std::string, SimpleQueue*> queue_map;
    void push(std::string, int priority, std::string* data);
    void push_postponed(std::string, int delay, std::string* data);
    std::string *pop(std::string);
    unsigned long length(std::string);
    void clear(std::string);
    std::string getQueueList();
};


#endif //LINUX_DEAMON_NAMEDQUEUE_H
