#ifndef HTTP_DEFAULTMOULDE_H
#define HTTP_DEFAULTMOULDE_H

#include "minix/module.h"

namespace ikk {

class DefaultModule : public Module {
public:
    DefaultModule();

    virtual ~DefaultModule() {}

    virtual bool handle(Event* ev, char* buf, int len);

    virtual bool config();

private:
    std::string htmlPath_;
    char* buf_;
};
}

#endif //HTTP_DEFAULTMOULDE_H
