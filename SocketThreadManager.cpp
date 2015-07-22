//
// Created by wlad on 7/18/15.
//

#include <stdlib.h>

#include <string.h>
#include <thread>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

#include "SocketThreadManager.h"
#include "Options.h"
#include "Logger.h"
#include "WorkProc.h"
#include "CmdController.h"

#define END_OF_STREAM 0

int SocketThreadManager::startListenServerSocket() {
    struct addrinfo hostinfo, *res;

    int ret;
    int yes = 1;

// first, load up address structs with getaddrinfo():

    memset(&hostinfo, 0, sizeof(hostinfo));

    hostinfo.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hostinfo.ai_socktype = SOCK_STREAM;
    hostinfo.ai_flags = AI_PASSIVE;     // fill in my IP for me

    getaddrinfo(NULL, SERVER_PORT_TO_LISTEN, &hostinfo, &res);


    _server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    //if(server_fd < 0) throw some error;

    //prevent "Error Address already in use"
    ret = setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    // if(ret < 0) throw some error;

    ret = bind(_server_fd, res->ai_addr, res->ai_addrlen);

    if (ret != 0) {
        Logger::getInstance().WriteLog("[DAEMON] bind port error\n");
        return -1;
    }
    ret = listen(_server_fd, 50);
    if (ret == -1) {
        Logger::getInstance().WriteLog("[DAEMON] listen error\n");
        return -1;
    }

    return _server_fd;
}

void SocketThreadManager::CloseServerSocket() {
    close(_server_fd);
}

void SocketThreadManager::NewThread() {
    std::thread th(&SocketThreadManager::_workTread, this);
    th.detach();
}

void SocketThreadManager::_workTread() {
    //long long sig1 = reinterpret_cast<long long> (arg);
    //int server_fd =static_cast<int>(sig1);
    pthread_t pthread_t1;
    int rfd;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (;;) {
        rfd = _server_establish_connection();
        std::thread th(&SocketThreadManager::_tcp_server_read, this, rfd);
        th.detach();
    }
#pragma clang diagnostic pop
}
int SocketThreadManager::_server_establish_connection()
// This function will establish a connection between the server and the
// client. It will be executed for every new client that connects to the server.
// This functions returns the socket filedescriptor for reading the clients data
// or an error if it failed.
{
    char ipstr[INET6_ADDRSTRLEN];
    int port;


    int new_sd;
    struct sockaddr_storage remote_info;
    socklen_t addr_size;

    addr_size = sizeof(addr_size);
    new_sd = accept(_server_fd, (struct sockaddr *) &remote_info, &addr_size);

    getpeername(new_sd, (struct sockaddr *) &remote_info, &addr_size);

    // deal with both IPv4 and IPv6:
    if (remote_info.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *) &remote_info;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    } else { // AF_INET6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *) &remote_info;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    }


    return new_sd;

}

void SocketThreadManager::_tcp_server_read(int rfd)
/// This function runs in a thread for every client, and reads incomming data.
/// It also writes the incomming data to all other clients.
{

    char buf[MAX_DATA_SIZE];
    for (;;) {
        ssize_t buflen=0;
        ssize_t bytes;
        try {
            for(;;) {
                //read incomming message.
                bytes = recv(rfd, buf + buflen, sizeof(buf) - buflen - 1, 0);
                if (bytes == 0) break;
                if (bytes < 0) {
                    // error
                    close(rfd);
                    return;
                }
                buflen += bytes;
                if (buf[buflen-1] == END_OF_STREAM) {
                    // correct end of stream
                    break;
                }
                if (buflen >= sizeof(buf)-1) {
                    // length of stream is too large (overfull)
                    close(rfd);
                    return;
                }


            }
            Logger::getInstance().WriteLog(buf);
            if (buflen <= 2) {
                close(rfd);
                //exit and close thread
                return;
            }
            CmdController controller(buf, buflen);
            if(controller.Execute() == CMD_ERROR) {
                Logger::getInstance().WriteLog(controller.getResult());
            }
            send(rfd, controller.getResult().data(), controller.getResult().size() + 1, 0);


        }catch (const std::exception &ex)
        {
            Logger::getInstance().WriteLog(ex.what());
        }
    }
    close(rfd);
}