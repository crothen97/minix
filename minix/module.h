
#ifndef HTTP_MOUDLE_H
#define HTTP_MOUDLE_H

#include <ikkcpr/type.h>
#include <string>

namespace ikk {

class Event;

class Module {
    DISALLOW_COPY_AND_ASSIGN(Module);

public:
    Module() : next_(NULL) {}

    Module(const char* name) : name_(name), next_(NULL) {}

    virtual ~Module() {}

    virtual bool handle(Event* ev, char* buf, int len) = 0;

    virtual bool config();

    static Module* find(std::string name);

protected:
    std::string name_;
    Module* next_;
};

extern Module** modules;

}


#endif //HTTP_MOUDLE_H
