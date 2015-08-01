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
#define CMD_POSTPONED 't'
#define CMD_LIST 'q'
#define CMD_CLEAR 'e'
#define CMD_CLEAR_ALL 'a'

#define CMD_COUNTERS_GET 'G'
#define CMD_COUNTERS_INC 'I'
#define CMD_COUNTERS_DEC 'D'
#define CMD_COUNTERS_CLEAR 'E'
#define CMD_COUNTERS_LIST 'Q'


#define CMD_SUCCESS 0
#define CMD_ERROR 10

class CmdController
{

public:
    CmdController(char buf[], ssize_t buflen);
    int Execute();
    std::string getResult() {return _result;}

private:
    std::string _getQueueName();
    std::string _queueName;
    char *_buf;
    ssize_t _buflen;
    char _cmd;

    int _getPriority();
    int _getTime();
    std::string _result;
    std::string *_getData();
    std::string *_data;
};


#endif //LINUX_DEAMON_CMDCONTROLLER_H
