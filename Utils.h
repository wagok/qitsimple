//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_UTILS_H
#define LINUX_DEAMON_UTILS_H
#include <string>

namespace Utils {
    int SetFdLimit(int MaxFd);
    void SetPidFile(std::string Filename);
    void RemovePidFile(std::string Filename);
};


#endif //LINUX_DEAMON_UTILS_H
