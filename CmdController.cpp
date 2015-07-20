//
// Created by wlad on 7/18/15.
//

#include "CmdController.h"
#include "WorkProc.h"
#include "Logger.h"

CmdController::CmdController(char buf[], ssize_t buflen) {

    _buf = buf;
    _buflen = buflen;
    _cmd = buf[0];
}


int CmdController::Execute() {

    int return_code = CMD_SUCCESS;
    try {
        switch (_cmd) {
            case CMD_PUT: {
                WorkProc::getInstance().getQueue()->push(_getQueueName(), _getPriority(), _getData());
                _result = std::string("ok\n");
                break;
            }
            case CMD_GET: {
                std::string *data = WorkProc::getInstance().getQueue()->pop(_getQueueName());
                if (data != NULL) {
                    _result = std::string(*data);
                    delete data;
                } else {
                    _result = std::string("::no tasks::\n");
                }
                break;
            }

            case CMD_POSTPONED: {
                WorkProc::getInstance().getQueue()->push_postponed(_getQueueName(), _getTime(), _getData());
                _result = std::string("ok\n");
                break;
            }

            case CMD_LENGTH: {
                _result = std::to_string(WorkProc::getInstance().getQueue()->length(_getQueueName()));

                break;
            }
            case CMD_STATUS:
                break;

            default: {
                _result = std::string("Unknown command\n");
                return_code = CMD_ERROR;
                break;
            }
        }
    } catch (const std::exception &ex) {
        _result = std::string(ex.what());
        return_code = CMD_ERROR;
    }
    return return_code;
}

std::string CmdController::_getQueueName() {
    char name_len;
    name_len = _buf[1];
    return _queueName = std::string(_buf, 2, name_len);
}


int CmdController::_getPriority() {
    //return _buf[_buf[1] + 2];
    return _getTime();
}

int CmdController::_getTime() {
    int time;
    time = _buf[_buf[1] + 2];
    time <<= 8;
    time += _buf[_buf[1] + 3];
    return time;
}

std::string *CmdController::_getData() {
    return _data = new std::string(_buf + _buf[1] + 4, _buflen - _buf[1] - 4);
}
