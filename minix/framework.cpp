#include <ikkcpr/path.h>
#include "minix/framework.h"

using std::vector;
using std::string;

namespace ikk {

ikk::Config* configer = NULL;

Framework::Framework() {
    servers_ = NULL;
}

Framework::~Framework() {
    if (servers_) {
        delete[] servers_;
        servers_ = NULL;
    }

    if (configer) {
        delete configer;
        configer = NULL;
    }
}

bool Framework::config() {
    if (!configer) {
        if (!ikk::Path::exists(MINIHTTP_CONF_PATH)) {
            LOG_ERROR << "cannot find " << MINIHTTP_CONF_PATH;
            return false;
        }

        configer = new ikk::Config(MINIHTTP_CONF_PATH);

        if (!configer) {
            LOG_FATAL << "alloc memory failed";
            return false;
        }
    }

    vector<string> vs = configer->getStringVector("servers");
    serCnt_ = vs.size();

    if (serCnt_ == 0) {
        LOG_ERROR << "no server is configed, we have nothing to do";
        return false;
    }

    servers_ = new Server[serCnt_];

    if (!servers_) {
        LOG_FATAL << "alloc memory failed";
        return false;
    }

    for (int i = 0; i < serCnt_; ++i) {
        servers_[i].setName(vs[i]);
        if (!servers_[i].config()) {
            return false;
        }
    }

    return true;
}

void Framework::start() {
    for (int i = 0; i < serCnt_; ++i) {
        if (servers_[i].listen()) {
            // base_.add 会检查sp是否是nullptr
            ikk::Event_ptr sp = ikk::Event::listenerEvent(servers_[i].getSocket(),
                                                          STD::bind(&Server::onAccept, servers_ + i,
                                                                    STD::placeholders::_1));
            if (!base_.add(sp)) {
                LOG_ERROR << "add listener failed, server name is " << servers_[i].getName();
            }
        }
    }

    base_.dispatch();
}

}