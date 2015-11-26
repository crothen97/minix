#include "minix/module.h"
#include "minix/framework.h"
#include "minix/defaultmodule.h"

namespace ikk {

static DefaultModule theDefaultModule;

Module* modulesArr[] = {
        &theDefaultModule,
        NULL
};

Module** modules = modulesArr;

bool Module::config() {
    if (!configer) {
        return false;
    }

    ConfigSubObj_ptr obj = configer->getSubObject(name_.c_str());

    if (!obj) {
        LOG_WARN << "get " << name_ << " module failed";
        return true;
    }

    next_ = Module::find(obj->getString("module"));
    if (next_) {
        return next_->config();
    }

    return true;
}

Module* Module::find(std::string name) {
    for (int i = 0; modules[i]; ++i) {
        if (modules[i]->name_ == name) {
            return modules[i];
        }
    }

    return NULL;
}

}