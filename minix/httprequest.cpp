#include "minix/httprequest.h"
#include <ikkcpr/logging.h>
#include <ikkcpr/strutil.h>

using namespace ikk;

static const char* strMethods[] = {
    "GET",
    "POST",
    "PUT",
    "DELETE"
};

static const char* strHeaders[] = {
    "Host:",
    "Connection:",
    "User-Agent:",
    "Accept-Encoding:",
    "Accept:"
};

HttpRequest::HttpRequest(Method method, bool bClient) : method_(method) {
    if (bClient) {
        headers_[Connection] << "Keep-Alive";
        headers_[UserAgent]
            << "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36";
        headers_[AcceptCoding] << "identity";
        headers_[Accept] << "*/*";
    }

    keepAlive_ = false;
}

void HttpRequest::addPath(const char* path, size_t len) {
    path_.reset();
    path_.append(path, len);
}

void HttpRequest::addPath(const char* path) {
    path_.reset();
    path_ << path;
}

void HttpRequest::toString(ikk::MStream& ms) {
    CHECK_ERROR(method_ < MethodInvalid, "http request method is invalid");
    ms << strMethods[method_] << " " << path_.data() << " HTTP/1.0\r\n";

    for (HeadMap::iterator iter = headers_.begin(); iter != headers_.end(); ++iter) {
        ms << strHeaders[iter->first] << " " << iter->second << "\r\n";
    }

    ms << "\r\n";
}

void HttpRequest::addHead(Head type, const char* content, size_t len) {
    if (type > Accept) {
        LOG_WARN << "invalid http head type, addHead failed";
        return;
    }

    headers_[type].reset();
    headers_[type].append(content, len);
}

void HttpRequest::addHead(Head type, const char* content) {
    if (type > Accept) {
        LOG_WARN << "invalid http head type, addHead failed";
        return;
    }

    headers_[type].reset();
    headers_[type] << content;
}

bool HttpRequest::parseReqLine(const char* buf, size_t len) {
    const char* end = buf + len;

    if (buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T' && buf[3] == ' ') {
        method_ = MethodGet;
        buf += 4;
    } else if (buf[0] == 'P' && buf[1] == 'O' && buf[2] == 'S' && buf[3] == 'T' && buf[4] == ' ') {
        method_ = MethodPost;
        buf += 5;
    } else {
        method_ = MethodInvalid;
        return false;
    }

    while (*buf == ' ') {
        ++buf;
    }

    if (std::equal(end - 9, end, " HTTP/1.1")) {
        end -= 10;
        while (*end == ' ') {
            --end;
        }

        if (end >= buf) {
            addPath(buf, end - buf + 1);
            return true;
        }
    }

    return false;
}

void HttpRequest::parseHeader(const char* buf, size_t len) {
    const char* end = buf + len - 1;
    for (size_t i = 0; i < sizeof(strHeaders) / sizeof(char*); ++i) {
        size_t len = strlen(strHeaders[i]);
        if (std::equal(buf, buf + len, strHeaders[i])) {
            buf += len;
            while (*buf == ' ') {
                ++buf;
            }

            while (*end == ' ') {
                --end;
            }

            if (end >= buf) {
                size_t len2 = end - buf + 1;
                addHead((Head) i, buf, end - buf + 1);
                if (i == Connection) {
                    // FIXME: 不区分大小写
                    if (len2 == 10 && std::equal(buf, end + 1, "Keep-Alive")) {
                        keepAlive_ = true;
                    }
                }
            }
            return;
        }
    }
}