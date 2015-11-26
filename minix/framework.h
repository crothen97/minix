
#ifndef HTTP_FRAMEWORK_H
#define HTTP_FRAMEWORK_H

#include <ikkcpr/type.h>
#include <ikkcpr/config.h>
#include <ikkcpr/eventbase.h>
#include "minix/server.h"

#ifndef MINIHTTP_CONF_PATH
#define MINIHTTP_CONF_PATH "/home/crothen/minihttp.conf"
#endif

namespace ikk {
class Framework {
    DISALLOW_COPY_AND_ASSIGN(Framework);

public:
    Framework();

    ~Framework();

    bool config();

    void start();

    bool addEvent(ikk::Event_ptr event) { return base_.add(event); }

private:
    Server* servers_;
    int serCnt_;
    ikk::EventBase base_;
};

extern ikk::Config* configer;
}

#endif //HTTP_FRAMEWORK_H
