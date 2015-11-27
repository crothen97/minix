#include <ikkcpr/logging.h>
#include "minix/httpresponse.h"

using namespace ikk;

HttpResponse::HttpResponse() : status_(S_404_NotFound), keepAlive_(false) {
    headers_ << "Server" << ": " << "ikkcpr" << "\r\n";
}

void HttpResponse::addHeader(const char* name, const char* content) {
    headers_ << name << ": " << content << "\r\n";
}

void HttpResponse::setContentType(ContentType type) {
    // TODO
    const char* content;
    switch (type) {
        case CT_HTML:
            content = "text/html";
            break;

        default:
            content = "text/html";
            break;
    }
}

void HttpResponse::get(ikk::MStream& response) {
    char buf[128];
    ikk::MStream ms(buf, 128);
    switch (status_) {
        case S_200_OK:
            ms << "200 OK";
            break;

        case S_400_BadRequest:
            ms << "400 Bad HttpRequest";
            break;

        case S_404_NotFound:
            ms << "200 OK";
            // TODO body_ << "404 Not Found";
            break;

        default:
            ms << "403 Forbidden";
            break;

    }
    response.fmt("HTTP/1.1 %s\r\n", ms.data());
    response << headers_.data();
    if (keepAlive_) {
        response << "Connection: Keep-Alive\r\n";
    } else {
        response << "Connection: close\r\n";
    }
}