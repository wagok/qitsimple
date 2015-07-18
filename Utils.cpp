//
// Created by wlad on 7/18/15.
//

#include "Utils.h"
#include "Logger.h"

#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

int Utils::SetFdLimit(int MaxFd)
{
    struct rlimit lim;
    int           status;

    // зададим текущий лимит на кол-во открытых дискриптеров
    lim.rlim_cur = MaxFd;
    // зададим максимальный лимит на кол-во открытых дискриптеров
    lim.rlim_max = MaxFd;

    // установим указанное кол-во
    status = setrlimit(RLIMIT_NOFILE, &lim);

    return status;
}
void Utils::SetPidFile(std::string Filename)
{
    FILE* f;

    f = fopen(Filename.data(), "w+");
    if (f)
    {
        fprintf(f, "%u", getpid());
        fclose(f);
    } else {
        Logger::getInstance().WriteLog("[MONITOR] Can't open pid file (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void Utils::RemovePidFile(std::string Filename) {
    unlink(Filename.data());
}