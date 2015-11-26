#ifndef HTTP_DEFAULTMOULDE_H
#define HTTP_DEFAULTMOULDE_H

#include "minix/module.h"

namespace ikk {

class HttpRequest;

class DefaultModule : public Module {
public:
    DefaultModule();

    virtual ~DefaultModule() {}

    virtual bool handle(Event* ev, char* buf, int len);
};
}

#endif //HTTP_DEFAULTMOULDE_H
