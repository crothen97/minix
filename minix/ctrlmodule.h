
#ifndef HTTP_CTRLMODULE_H
#define HTTP_CTRLMODULE_H

#include "minix/module.h"

namespace ikk {

class CtrlModule : public Module {
public:
    CtrlModule();

    virtual ~CtrlModule() {}

    virtual bool handle(Event* ev, char* buf, int len);
};

}

#endif //HTTP_CTRLMODULE_H
