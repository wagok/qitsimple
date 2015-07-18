//
// Created by wlad on 7/18/15.
//

#include "Logger.h"
#include <syslog.h>
void Logger::WriteLog(std::string Msg, ...) {

    openlog("qitsimple", LOG_PID|LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "%s", Msg.data());
    closelog();
}
