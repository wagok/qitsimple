//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_SOCKETTHREAD_H
#define LINUX_DEAMON_SOCKETTHREAD_H




class SocketThreadManager {
public:
    static SocketThreadManager& getInstance()
    {
        static SocketThreadManager    instance;

        return instance;
    }
    void NewThread();
    int startListenServerSocket();
private:

    static int _server_fd;
    void _workTread();
    int _server_establish_connection();
    void _tcp_server_read(int rfd);
    SocketThreadManager() {};

    SocketThreadManager(SocketThreadManager const&)       = delete;
    void operator=(SocketThreadManager const&)   = delete;


};


#endif //LINUX_DEAMON_SOCKETTHREAD_H
