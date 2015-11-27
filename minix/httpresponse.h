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

    void setBody(MStream& ms, const char* body) { ms << "Content-Length: " << strlen(body) << "\r\n\r\n" << body; }

    void setBody(MStream& ms, const char* body, size_t len) { ms << "Content-Length: " << len << "\r\n\r\n" << body; }

    void setBody(MStream& ms, ikk::MStream& body) { ms << "Content-Length: " << body.size() << "\r\n\r\n" << body; }

    // void resetBody() { body_.reset(); }

    Status status() const { return status_; }

private:
    MStream headers_;
    Status status_;
    bool keepAlive_;
    // ikk::MStream body_;
};

}

#endif //HTTP_RESPONSE_H
