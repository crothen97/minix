#include "minix/server.h"
#include "minihttp/framework.h"
#include "httprequest.h"
#include "minihttp/module.h"

using namespace ikk;

extern Framework* theFramework;

Server::Server() {
    timeout_ = -1;
    module_ = NULL;
}

Server::~Server() {
    listenfd_.close();
}

void Server::setName(std::string& name) {
    name_.swap(name);
}

bool Server::config() {
    if (!configer) {
        return false;
    }

    ConfigSubObj_ptr obj = configer->getSubObject(name_.c_str());

    qint32 tmp;
    if (obj->getInt("port", tmp)) {
        port_ = (quint16) tmp;
    } else {
        port_ = 0;
        LOG_ERROR << "get port failed, server name is " << name_;
        return false;
    }

    if (obj->getInt("timeout", tmp)) {
        timeout_ = tmp;
    }

    module_ = Module::find(obj->getString("module"));
    if (!module_) {
        module_ = Module::find("default");
    }

    return module_->config();
}

bool Server::listen() {
    if (port_) {
        SockAddr addr(port_);

        if (listenfd_.reuseAddr(1) != 0) {
            LOG_ERROR << "reuseAddr failed,  port = " << port_;
            return false;
        }

        if (listenfd_.bind(addr) != 0) {
            LOG_ERROR << "bind port " << port_ << " failed";
            return false;
        }

        if (listenfd_.listen() != 0) {
            LOG_ERROR << "listen failed, port = " << port_;
            return false;
        }
    }

    return true;
}

void Server::onAccept(Event* ev) {
    Socket sc = ev->accept();

    if (!sc.valid()) {
        LOG_ERROR << "accept failed, server name is " << name_;
        return;
    }

    EvRWCallback cb = STD::bind(&Server::onRead, this, STD::placeholders::_1,
                                     STD::placeholders::_2);

    Event_ptr sp = STD::make_shared<Event>(sc, IKK_EV_READ, timeout_, cb);
    if (!theFramework->addEvent(sp)) {
        LOG_ERROR << "add event failed, server name is " << name_;
        return;
    }
}

bool Server::onRead(Event* ev, bool bTimeout) {
    if (bTimeout) {
        return true;
    }

    int len;
    char* buf = ev->recv(len);
    if (len <= 0) {
        return false;
    }

    return module_->handle(ev, buf, len);
}
