#if !defined(_GNU_SOURCE)
        #define _GNU_SOURCE
#endif

#include <iostream>
#include "DaemonProc.h"

std::string echo(std::string* str);

int main(int argc, char** argv)
{
    /*    char buf[] = {'p', 5, 'h', 'e', 'l', 'l', 'o', 12, 'd', 'a', 't', 'a', ';'};
       int buflen = 13;
       char name_len;
       char priority;
       name_len = buf[1];
       std::string name(buf, 2, name_len);
       priority = buf[name_len + 2];
       std::string *data = new std::string(buf, name_len + 3, buflen - name_len - 3);
       std::cout << "Command:" << buf[0] << "; Name length:" << std::to_string(name_len) << "; Name:" << name << "; Priority:" << std::to_string(priority) << " Data:" << *data;
       exit;
      NamedQueue *queue = new NamedQueue;
       queue->push("queue_1", 1, new std::string("1,2,3,4,5"));
       queue->push("queue_2", 1, new std::string("1,2,3,4,5,6,7,8,9"));
       queue->push("queue_2", 2, new std::string("1,2"));
       queue->push("queue_2", 3, new std::string("1,2,3"));


       queue->push("queue_1", 2, new std::string("1,2,3,4,5"));
       queue->push("queue_2", 1, new std::string("1,2,3,4,5,6,7,8,9"));
       queue->push("queue_2", 1, new std::string("1,2"));
       queue->push("queue_2", 1, new std::string("1,2,3"));
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_1")) << "\n";
       std::cout << echo(queue->pop("queue")) << "\n";
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_2")) << "\n";
       std::cout << echo(queue->pop("queue_1")) << "\n";
       std::cout << echo(queue->pop("queue")) << "\n";
       return 0;
   */
    if (argc != 2)
    {
        printf("Usage: ./qitsimple filename.cfg\n");
        return -1;
    }

    return DaemonProc::getInstance().startDaemon(argv[1]);


}
/*
std::string echo(std::string* str) {
    if (str != NULL) {
        return *str;
    } else {
        return std::string("NULL");
    }
}
 */
 