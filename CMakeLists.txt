cmake_minimum_required(VERSION 3.2)
project(qitsimple)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package (Threads)
set(SOURCE_FILES main.cpp)
add_executable(qitsimple ${SOURCE_FILES} logger.h logger.cpp config.h config.cpp work.h work.cpp options.h options.cpp monitor.h monitor.cpp SimpleQueue.cpp SimpleQueue.h NamedQueue.cpp NamedQueue.h DaemonProc.cpp DaemonProc.h Logger.cpp Logger.h ConfigManager.cpp ConfigManager.h WorkProc.cpp WorkProc.h Options.h Utils.cpp Utils.h SocketThreadManager.cpp SocketThreadManager.h CmdController.cpp CmdController.h)
target_link_libraries (qitsimple ${CMAKE_THREAD_LIBS_INIT})