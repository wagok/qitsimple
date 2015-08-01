//
// Created by root on 8/2/15.
//

#ifndef QITSIMPLE_COUNTERSMAP_H
#define QITSIMPLE_COUNTERSMAP_H

#include <string>
#include <unordered_map>
#include <mutex>
#include "SimpleQueue.h"

class CountersMap {

    private:
        std::mutex _lock;
    public:
        std::unordered_map<std::string, long long> counters_map;
        long long get(std::string);
        void inc(std::string);
        void dec(std::string);
        void clear(std::string);
        std::string getList();
};


#endif //QITSIMPLE_COUNTERSMAP_H
