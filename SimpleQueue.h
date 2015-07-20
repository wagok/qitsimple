//
// Created by root on 7/16/15.
//

#ifndef LINUX_DEAMON_SIMPLEQUEUE_H
#define LINUX_DEAMON_SIMPLEQUEUE_H


#include <string>
#include <queue>
#include <mutex>

struct QueueElem {

    int priority;
    int timestamp;
    std::string * data;
    QueueElem(int priority, int timestamp, std::string *data) {
        this->data = data;
        this->priority = priority;
        this->timestamp = timestamp;
    };
};

class CompareElem {
public:
    bool operator()(QueueElem*& t1, QueueElem*& t2)
    {
        if (t1->priority < t2->priority) return true;
        if (t1->timestamp > t2->timestamp) return true;
        return false;
    }
};

class SimpleQueue {

private:
    std::mutex _lock;
public:
    std::priority_queue<QueueElem*, std::vector<QueueElem*>, CompareElem> queue;
    void push(int priority, std::string* data);
    void push_postponed(int timestamp, std::string *data);
    std::string *pop();
    unsigned long length();
};


#endif //LINUX_DEAMON_SIMPLEQUEUE_H
