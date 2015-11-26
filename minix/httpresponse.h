#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <ikkcpr/mstream.h>

namespace ikk {

class HttpResponse {
    DISALLOW_COPY_AND_ASSIGN(HttpResponse);

public:
    enum Status {
        S_200_OK = 200,
        S_400_BadRequest = 400,
        S_404_NotFound = 404
    };

    enum ContentType {
        CT_HTML = 0,
    };

    HttpResponse();

    ~HttpResponse() { }

    void setStatus(Status status) { status_ = status; }

    void enableKeepalive(bool on) { keepAlive_ = on; }

    bool keepalive() const { return keepAlive_; }

    void get(ikk::MStream& response);

    void addHeader(const char* name, const char* content);

    void setContentType(ContentType type);

    void setBody(const char* body) { body_ << body; }

    void setBody(ikk::MStream& body) { body_ << body; }

    void resetBody() { body_.reset(); }

    Status status() const { return status_; }

private:
    ikk::MStream headers_;
    Status status_;
    bool keepAlive_;
    ikk::MStream body_;
};

}

#endif //HTTP_RESPONSE_H
