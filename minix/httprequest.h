#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <ikkcpr/mstream.h>
#include <map>
#include <string>

namespace ikk {

class HttpRequest {
    DISALLOW_COPY_AND_ASSIGN(HttpRequest);

public:
    enum Method {
        MethodGet = 0,
        MethodPost = 1,
        MethodPut = 2,
        MethodDELETE = 3,
        MethodInvalid,
    };

    enum Head {
        Host = 0,
        Connection = 1,
        UserAgent = 2,
        AcceptCoding = 3,
        Accept = 4,
    };

    typedef std::map<Head, ikk::MStream> HeadMap;

    explicit HttpRequest(Method method = MethodInvalid, bool bClient = false);

    ~HttpRequest() { }

    void setMethod(Method method) { method_ = method; }

    bool parse(const char* buf, size_t len);

    bool parseReqLine(const char* buf, size_t len);

    void parseHeader(const char* buf, size_t len);

    void addPath(const char* path, size_t len);

    void addPath(const char* path);

    void addHead(Head type, const char* content, size_t len);

    void addHead(Head type, const char* content);

    void toString(ikk::MStream& ms);

    bool keepAlive() const { return keepAlive_; }

    const HeadMap& headers() const { return headers_; }

    MStream& path() { return path_; }

private:
    Method method_;
    MStream path_;
    HeadMap headers_;
    bool keepAlive_;
};
}

#endif //HTTP_REQUEST_H
