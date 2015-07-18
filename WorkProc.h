//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_WORKPROC_H
#define LINUX_DEAMON_WORKPROC_H


#include "NamedQueue.h"

class WorkProc
{
public:
    static WorkProc& getInstance()
    {
        static WorkProc    instance;

        return instance;
    }
    int run();
    static void signalHandler(int sig, siginfo_t *si, void *ptr);
    NamedQueue *getQueue() { return _queue; }
private:
    WorkProc() {};

    NamedQueue * _queue;

    WorkProc(WorkProc const&)       = delete;
    void operator=(WorkProc const&)   = delete;

    int _initWorkThread();
    void _destroyWorkThread();
};
#endif //LINUX_DEAMON_WORKPROC_H
