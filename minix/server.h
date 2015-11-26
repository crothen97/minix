#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <ikkcpr/type.h>
#include <ikkcpr/socket.h>
#include <ikkcpr/event.h>
#include <string>

#define CTRLSERNAME "ctrl_server"

namespace ikk {

class HttpRequest;

class Module;

class Server {
    DISALLOW_COPY_AND_ASSIGN(Server);

public:
    Server();

    ~Server();

    void setName(std::string& name);

    void setName(const char* name);

    const std::string getName() const {return  name_;}

    bool config();

    bool listen();

    void onAccept(ikk::Event* ev);

    bool onRead(ikk::Event*ev, bool bTimeout);



    ikk::Socket getSocket() const {return listenfd_;}

private:
    std::string name_;
    ikk::Socket listenfd_;
    ikk::quint16 port_;
    int timeout_;
    Module* module_;
};

}


#endif //HTTP_SERVER_H
