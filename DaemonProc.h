//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_DEAMONPROC_H
#define LINUX_DEAMON_DEAMONPROC_H

#include <string>

#include "Logger.h"

class DaemonProc
{
public:
    static DaemonProc & getInstance()
    {
        static DaemonProc instance;

        return instance;
    }
    int startDaemon(std::string configFileStr);
private:
    DaemonProc() {
    };
    int _monitorProc();
    int _workProc();
    DaemonProc(DaemonProc const&)       = delete;
    void operator=(DaemonProc const&)   = delete;
};

#endif //LINUX_DEAMON_DEAMONPROC_H