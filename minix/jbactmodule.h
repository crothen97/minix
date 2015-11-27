
#ifndef HTTP_JBACTMODULE_H
#define HTTP_JBACTMODULE_H

#include "minix/module.h"

namespace ikk {

class JbactModule : public Module {
public:
    JbactModule();

    virtual ~JbactModule() {}

    virtual bool handle(Event* ev, char* buf, int len);
};

}

#endif //HTTP_JBACTMODULE_H
