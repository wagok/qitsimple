//
// Created by wlad on 7/18/15.
//

#ifndef LINUX_DEAMON_CONFIGMANAGER_H
#define LINUX_DEAMON_CONFIGMANAGER_H

#include <string>

class ConfigManager
{
public:
    static ConfigManager& getInstance()
    {
        static ConfigManager    instance;

        return instance;
    }
    int LoadConfig(std::string configName);
    int ReloadConfig();
private:
    ConfigManager() {};

    ConfigManager(ConfigManager const&)       = delete;
    void operator=(ConfigManager const&)   = delete;

    std::string configName;
};


#endif //LINUX_DEAMON_CONFIGMANAGER_H
