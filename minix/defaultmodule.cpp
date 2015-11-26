#include <ikkcpr/strutil.h>
#include <ikkcpr/logging.h>
#include <ikkcpr/event.h>

#include "minix/defaultmodule.h"
#include "minix/httprequest.h"
#include "minix/httpresponse.h"

using namespace ikk;

DefaultModule::DefaultModule() : Module("default") {}

bool DefaultModule::handle(Event* ev, char* buf, int len) {
    size_t idxs[128];
    size_t count = 0;

    finds(buf, "\r\n", idxs, &count);

    if (!count) {
        LOG_ERROR << "parse failed\n";
        return false;
    }

    HttpRequest req;
    if (!req.parseReqLine(buf, idxs[0])) {
        return false;
    }

    const char* start = buf + idxs[0] + 2;
    for (size_t i = 1; i < count; ++i) {
        req.parseHeader(start, idxs[i] - idxs[i - 1] - 2);
        start = buf + idxs[i] + 2;
    }

    HttpResponse res;
    if (req.path().equal("/")) {
        res.setStatus(HttpResponse::S_200_OK);
        res.setContentType(HttpResponse::CT_HTML);
        res.setBody("welcom to ikkcpr http server!");
    }

    MStream ms;
    res.get(ms);

    ev->send(ms.data(), ms.size());
    if (!res.keepalive()) {
        ev->shutdownAll();
    }

    return true;
}
