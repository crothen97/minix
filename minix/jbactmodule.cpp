#include "minix/jbactmodule.h"
#include "minix/httprequest.h"
#include "minix/httpresponse.h"
#include "minix/framework.h"


using namespace ikk;

JbactModule::JbactModule() : Module("jbactmod") {}

bool JbactModule::handle(Event* ev, char* buf, int len) {
    HttpRequest req;
    if (!req.parse(buf, len)) {
        return false;
    }

    const char* p = fastsearch(req.path().data(), req.path().size(), "?", 1);

    char ansBuf[1024];
    MStream ansMs(ansBuf, 1024);

    if (p) {
        p = fastsearch(p, strlen(p), "salt=", 5);
        if (p) {
            p += 5;
            const char* p1 = fastsearch(p, strlen(p), "&", 1);
            if (p1) {
                ansMs << "<!-- 23c0387dca7faa7c847e161940a9532950af7ca6a8b6fd836042aa8c79d84ae57856d4d06cf469569cfce545aeb712fcc616bd5c0249495e0daeb7f4102a7a5c -->\n"
                        "<ObtainTicketResponse><message></message><prolongationPeriod>607875500</prolongationPeriod><responseCode>OK</responseCode><salt>";
                ansMs.append(p, p1 - p);
                ansMs << "</salt><ticketId>1</ticketId><ticketProperties>licensee=crothen\tlicenseType=0\t</ticketProperties></ObtainTicketResponse>";
            }
        }
    }

    HttpResponse res;
    MStream ms;

    res.get(ms);
    if (ansMs.size() > 0) {
        res.setBody(ms, ansMs);
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