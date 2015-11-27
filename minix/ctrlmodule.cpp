#include <ikkcpr/mstream.h>
#include <ikkcpr/event.h>

#include "minix/ctrlmodule.h"
#include "minix/httprequest.h"
#include "minix/httpresponse.h"
#include "minix/framework.h"

using namespace ikk;

CtrlModule::CtrlModule() : Module("ctrlmod") { }

bool CtrlModule::handle(Event* ev, char* buf, int len) {
    HttpRequest req;
    if (!req.parse(buf, len)) {
        return false;
    }

    HttpResponse res;
    if (req.path().equal("/stop")) {
        res.setStatus(HttpResponse::S_200_OK);
        res.setContentType(HttpResponse::CT_HTML);
        res.setBody("server is stopping!");

        theFramework->stop();
    } else if (req.path().equal("/restart")) {
        // TODO
    }

    MStream ms;
    res.get(ms);

    ev->clearBuf();

    ev->send(ms.data(), ms.size());
    if (!res.keepalive()) {
        ev->shutdownAll();
    }

    return true;
}