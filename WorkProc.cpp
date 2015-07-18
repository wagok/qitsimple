//
// Created by wlad on 7/18/15.
//
#include <stdlib.h>
#include <wait.h>

#include <string.h>
#include <execinfo.h>
#include <thread>

#include "WorkProc.h"
#include "Options.h"
#include "Logger.h"
#include "ConfigManager.h"

#include "Utils.h"
#include "SocketThreadManager.h"

int WorkProc::run() {

    struct sigaction sigact;
    sigset_t sigset;
    int signo;
    int status;

    // сигналы об ошибках в программе будут обрататывать более тщательно
    // указываем что хотим получать расширенную информацию об ошибках
    sigact.sa_flags = SA_SIGINFO;
    // задаем функцию обработчик сигналов
    sigact.sa_sigaction = signalHandler;

    sigemptyset(&sigact.sa_mask);

    // установим наш обработчик на сигналы

    sigaction(SIGFPE, &sigact, 0); // ошибка FPU
    sigaction(SIGILL, &sigact, 0); // ошибочная инструкция
    sigaction(SIGSEGV, &sigact, 0); // ошибка доступа к памяти
    sigaction(SIGBUS, &sigact, 0); // ошибка шины, при обращении к физической памяти

    sigemptyset(&sigset);

    // блокируем сигналы которые будем ожидать
    // сигнал остановки процесса пользователем
    sigaddset(&sigset, SIGQUIT);

    // сигнал для остановки процесса пользователем с терминала
    sigaddset(&sigset, SIGINT);

    // сигнал запроса завершения процесса
    sigaddset(&sigset, SIGTERM);

    // пользовательский сигнал который мы будем использовать для обновления конфига
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    // Установим максимальное кол-во дискрипторов которое можно открыть
    Utils::SetFdLimit(FD_LIMIT);

    // запишем в лог, что наш демон стартовал
    Logger::getInstance().WriteLog("[DAEMON] Started\n");

    // запускаем все рабочие потоки
    try {
        status = _initWorkThread();
    }catch (const std::exception &ex)
    {
        Logger::getInstance().WriteLog(ex.what());
    }
    if (status != -1) {
        // цикл ожижания сообщений
        for (; ;) {
            // ждем указанных сообщений
            sigwait(&sigset, &signo);

            // если то сообщение обновления конфига
            if (signo == SIGUSR1) {
                // обновим конфиг
                status = ConfigManager::getInstance().ReloadConfig();
                if (status == 0) {
                    Logger::getInstance().WriteLog("[DAEMON] Reload config failed\n");
                }
                else {
                    Logger::getInstance().WriteLog("[DAEMON] Reload config OK\n");
                }
            }
            else // если какой-либо другой сигнал, то выйдим из цикла
            {
                break;
            }
        }

        // остановим все рабочеи потоки и корректно закроем всё что надо
        _destroyWorkThread();
    }
    else {
        Logger::getInstance().WriteLog("[DAEMON] Create work thread failed\n");
    }

    Logger::getInstance().WriteLog("[DAEMON] Stopped\n");

    // вернем код не требующим перезапуска
    return CHILD_NEED_TERMINATE;
}

void WorkProc::signalHandler(int sig, siginfo_t *si, void *ptr) {
    void *ErrorAddr;
    void *Trace[16];
    int x;
    int TraceSize;
    char **Messages;

    // запишем в лог что за сигнал пришел
    Logger::getInstance().WriteLog("[DAEMON] Signal: %s, Addr: 0x%0.16X\n", strsignal(sig), si->si_addr);


#if __WORDSIZE == 64
    ErrorAddr = (void *) ((ucontext_t *) ptr)->uc_mcontext.gregs[REG_RIP];
#else
    ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
#endif

    TraceSize = backtrace(Trace, 16);
    Trace[1] = ErrorAddr;

    // получим расшифровку трасировки
    Messages = backtrace_symbols(Trace, TraceSize);
    if (Messages) {
        Logger::getInstance().WriteLog("== Backtrace ==\n");

        // запишем в лог
        for (x = 1; x < TraceSize; x++) {
            Logger::getInstance().WriteLog("%s\n", Messages[x]);
        }

        Logger::getInstance().WriteLog("== End Backtrace ==\n");
        free(Messages);
    }

    Logger::getInstance().WriteLog("[DAEMON] Stopped\n");

    WorkProc::getInstance()._destroyWorkThread();

    exit(CHILD_NEED_WORK);
}

int WorkProc::_initWorkThread() {
    _queue = new NamedQueue();

    int server_fd = SocketThreadManager::getInstance().startListenServerSocket();
    if (server_fd == -1) {
        return -1;
    }
    SocketThreadManager::getInstance().NewThread();
}

void WorkProc::_destroyWorkThread() {
    if (_queue != nullptr) {
        delete _queue;
    }
}

