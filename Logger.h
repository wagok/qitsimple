//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_LOGGER_H
#define LINUX_DEAMON_LOGGER_H

#include <string>

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger    instance;

        return instance;
    }
    void WriteLog(std::string Msg, ...);
private:
    Logger() {};

    Logger(Logger const&)       = delete;
    void operator=(Logger const&)   = delete;

};


#endif //LINUX_DEAMON_LOGGER_H
