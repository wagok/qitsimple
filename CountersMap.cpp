//
// Created by root on 8/2/15.
//

#include "CountersMap.h"

 long long CountersMap::get(std::string name) {

    _lock.lock();
    auto search = counters_map.find(name);
    if(search != counters_map.end()) {
        long long res = search->second;
        _lock.unlock();
        return res;
    }
    _lock.unlock();
    return 0LL;
}
void CountersMap::inc(std::string name) {

    _lock.lock();
    auto search = counters_map.find(name);
    if(search != counters_map.end()) {
        search->second++;
    } else {
        counters_map[name] = 1LL;
    }
    _lock.unlock();
}
void CountersMap::dec(std::string name) {

    _lock.lock();
    auto search = counters_map.find(name);
    if(search != counters_map.end()) {
        search->second--;
    } else {
        counters_map[name] = -1LL;
    }
    _lock.unlock();
}
void CountersMap::clear(std::string name) {

    _lock.lock();
    auto search = counters_map.find(name);
    if(search != counters_map.end()) {
        counters_map.erase(search);
    }
    _lock.unlock();
}

std::string CountersMap::getList() {
    std::string result("");
    for(auto elem : counters_map) {
        result.append(elem.first);
        result.append("\n");
    }
    return result;
}
