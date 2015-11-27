#include <ikkcpr/strutil.h>
#include <ikkcpr/logging.h>
#include <ikkcpr/event.h>
#include <ikkcpr/path.h>
#include <ikkcpr/kifstream.h>

#include "minix/defaultmodule.h"
#include "minix/httprequest.h"
#include "minix/httpresponse.h"
#include "minix/framework.h"

using namespace ikk;

#define BUFSIZE (1024 * 256)

DefaultModule::DefaultModule() : Module("defaultmod") {
    buf_ = new char[BUFSIZE];
    CHECK_FAIL(buf_, "new char array failed");
}

bool DefaultModule::handle(Event* ev, char* buf, int len) {
    HttpRequest req;
    if (!req.parse(buf, len)) {
        return false;
    }

    req.path().replaceFirst('?', '\0');
    MStream localPath;
    localPath << htmlPath_ << req.path();

    HttpResponse res;
    ssize_t bodyLen = 0;
    if (Path::exists(localPath.data())) {
        KIFStream kifs(localPath.data());
        bodyLen = kifs.read(buf_, BUFSIZE);
        if (bodyLen > 0) {
            res.setStatus(HttpResponse::S_200_OK);
            res.setContentType(HttpResponse::CT_HTML);
        } else {
            bodyLen = 0;
            LOG_WARN << "read " << localPath << " failed";
        }
    } else {
        LOG_WARN << "not exist";
    }

    MStream ms(((size_t) bodyLen & (~0xfff)) + 0x2000);

    res.get(ms);
    if (bodyLen > 0) {
        res.setBody(ms, buf_, (size_t) bodyLen);
    } else {
        res.setBody(ms, "404 Not Found");
    }

    ev->clearBuf();

    ev->send(ms.data(), ms.size());
    if (!res.keepalive()) {
        ev->shutdownAll();
    }

    return true;
}

bool DefaultModule::config() {
    if (!configer) {
        return false;
    }

    ConfigSubObj_ptr obj = configer->getSubObject(name_.c_str());

    if (!obj) {
        LOG_ERROR << "get " << name_ << " module failed";
        return false;
    }

    htmlPath_ = obj->getString("path");
    if (htmlPath_.empty()) {
        LOG_ERROR << "path is not set";
        return false;
    }

    return true;
}
