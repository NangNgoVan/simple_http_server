#include <iostream>

using namespace std;

#include "simple_server.h"

int main(int argc, char **argv)
{

    cout << "--- Demo simple HTTP Server ---" << endl;

    SimpleServer server;

    int port = 8080;

    try {

        server.addRequestHandler("/hello", HttpRequest::GET, [](HttpRequest &req, HttpResponse &res) {
            res.setStatusCode(HttpResponse::OK);
            res.setHeader("Content-Type", "text/plan");
            res.sendText("Hello world!");
        });

        server.addRequestHandler("/", HttpRequest::GET, [](HttpRequest &req, HttpResponse &res) {
            res.setHeader("Content-Type", "text/html");
            res.setHeader("Connection", "Keep-Alive");
            res.sendFile("index.html");
        });

        server.listen(port);
        cout << "Listening on host: " << server.getHost()
                            << ", port: " << port << "\n";

        // server.stop();
    }
    catch(Exception::InternalServerError &err)
    {
        cout << err.what() << "\n";
    }
    
    return 0;
}

