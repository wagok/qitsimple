#if !defined(_GNU_SOURCE)
        #define _GNU_SOURCE
#endif

#include <iostream>
#include "DaemonProc.h"

int main(int argc, char** argv)
{

    if (argc != 2)
    {
        printf("Usage: ./qitsimple filename.cfg\n");
        return -1;
    }

    return DaemonProc::getInstance().startDaemon(argv[1]);


}
