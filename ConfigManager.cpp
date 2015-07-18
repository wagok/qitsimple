//
// Created by wlad on 7/18/15.
//

#include "ConfigManager.h"

int ConfigManager::LoadConfig(std::string configName) {

    this->configName = configName;

    return 1;
}

int ConfigManager::ReloadConfig() {

    return 1;
}
