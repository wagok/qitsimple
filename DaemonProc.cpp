//
// Created by wlad on 7/18/15.
//
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <wait.h>

#include "Options.h"
#include "DaemonProc.h"
#include "ConfigManager.h"
#include "WorkProc.h"

#include "Utils.h"


int DaemonProc::startDaemon(std::string configFileStr) {

    int status;
    int pid;
    // load configuration file
    status = ConfigManager::getInstance().LoadConfig(configFileStr);

    if (!status)
    {
        Logger::getInstance().WriteLog("Error: Load config failed\n");
        return -1;
    }

    // create child
    pid = fork();

    if (pid == -1)
    {
        Logger::getInstance().WriteLog("Start Daemon Error: %s\n", strerror(errno));
        return -1;
    }
    else if (!pid) // for child process
    {

        // This  function will monitor daemon
        status = _monitorProc();

        return status;
    }
    else // for parent process
    {
        // finish process
        return 0;
    }
}

int DaemonProc::_monitorProc() {
    pid_t pid, sid;
    int       status;
    int       need_start = 1;
    sigset_t  sigset;
    siginfo_t siginfo;



    // file permissions
    umask(0);

    // create new session
    sid = setsid();
    if (sid < 0) {
        Logger::getInstance().WriteLog("Can't create new session (%s)\n", strerror(errno));
        return(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
        /* Log any failure here */
        return(EXIT_FAILURE);
    }

    // close in/out descriptions
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);


    // setup signals
    sigemptyset(&sigset);

    // quit signal by user
    sigaddset(&sigset, SIGQUIT);

    // quit signal by user from terminal
    sigaddset(&sigset, SIGINT);

    // quit request signal
    sigaddset(&sigset, SIGTERM);

    // change child status signal
    sigaddset(&sigset, SIGCHLD);

    // user signal to reload config
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    // create file with our PID
    Utils::SetPidFile(PID_FILE);

    for (;;)
    {
        // if need create child
        if (need_start)
        {
            Logger::getInstance().WriteLog("[MONITOR] Start child\n");
            // create child
            pid = fork();
        }

        need_start = 1;

        if (pid == -1) // if error occurred
        {
            Logger::getInstance().WriteLog("[MONITOR] Fork failed (%s)\n", strerror(errno));
        }
        else if (!pid) // if it is child
        {
            // this code is executed in child process

            // start daemon work process
            status = _workProc();

            // finish
            return(status);
        }
        else // if it is parent
        {
            // this code is executed in parent process

            // wait a signal
            sigwaitinfo(&sigset, &siginfo);

            // if signal come from child
            if (siginfo.si_signo == SIGCHLD)
            {
                // receiving exit code
                wait(&status);

                // transform exit code to friendly view
                status = WEXITSTATUS(status);

                // no need to restart child
                if (status == CHILD_NEED_TERMINATE)
                {
                    Logger::getInstance().WriteLog("[MONITOR] Childer stopped\n");
                    break;
                }
                else if (status == CHILD_NEED_WORK) // need to restart
                {
                    Logger::getInstance().WriteLog("[MONITOR] Childer restart\n");
                }
            }
            else if (siginfo.si_signo == SIGUSR1) // need to reload config
            {
                kill(pid, SIGUSR1); // send signal to child
                need_start = 0; // set flag no need to restart
            }
            else // other signal come
            {
                Logger::getInstance().WriteLog("[MONITOR] Signal %s\n", strsignal(siginfo.si_signo));

                // kill child process
                kill(pid, SIGTERM);
                status = 0;
                break;
            }
        }
    }
    Logger::getInstance().WriteLog("[MONITOR] Stopped\n");

    // remove PID file
    Utils::RemovePidFile(PID_FILE);

    return status;
}



int DaemonProc::_workProc() {
    return WorkProc::getInstance().run();
}
