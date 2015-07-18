//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_CMDCONTROLLER_H
#define LINUX_DEAMON_CMDCONTROLLER_H

#include<string>
#include <sys/types.h>

#define CMD_PUT 'p'
#define CMD_GET 'g'
#define CMD_LENGTH 'l'
#define CMD_STATUS 's'

#define CMD_SUCCESS 0
#define CMD_ERROR 0

class CmdController
{

public:
    CmdController(char *buf, ssize_t buflen);
    int Execute();
    std::string getResult() {return _result};

private:
    std::string _getQueueName();
    std::string _queueName;
    char _buf[];
    ssize_t _buflen;
    char _cmd;

    int _getPriority();
    std::string _result;
    std::string *_getData();
    std::string *_data;
};


#endif //LINUX_DEAMON_CMDCONTROLLER_H
