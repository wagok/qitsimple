//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_OPTIONS_H
#define LINUX_DEAMON_OPTIONS_H
#define PID_FILE "/var/run/qitsimple.pid"
// maximum descriptions limit
#define FD_LIMIT                        1024*10

// exit codes constants
#define CHILD_NEED_WORK         1
#define CHILD_NEED_TERMINATE    2
#define SERVER_PORT_TO_LISTEN    "5555"
#define MAX_DATA_SIZE 524288 /* 512K */

#endif //LINUX_DEAMON_OPTIONS_H
