#ifndef SIMPLE_HTTP_SERVER_H
#define SIMPLE_HTTP_SERVER_H

#include <iostream>
#include <functional>
#include <map>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#include "exceptions.h"
#include "http_request.h"
#include "http_response.h"

using namespace std;

typedef std::function<void(HttpRequest&, HttpResponse&)> REQUEST_HANDLER;

class SimpleServer
{
public:
    SimpleServer(string host = "0.0.0.0");
    ~SimpleServer();
    void listen(
        int port);
    
    void stop();

    void addRequestHandler(std::string path, 
        HttpRequest::HttpMethod method, 
        REQUEST_HANDLER handler);

private:
    string m_host;
    int m_sockFd;
    bool m_isRunning;

    std::map<std::string, 
        REQUEST_HANDLER> m_handlerMap;
        
    REQUEST_HANDLER *getRequestHandler(std::string path);

    void waitForClientConnection();

};

#endif
