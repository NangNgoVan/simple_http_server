#include <iostream>

using namespace std;

#include "simple_server.h"

int main(int argc, char **argv)
{

    cout << "--- Demo simple HTTP Server ---" << endl;

    SimpleServer server;

    string host = "0.0.0.0";
    int port = 8080;

    try {

        server.addRequestHandler("/", HttpRequest::GET, [](HttpRequest &req, HttpResponse &res) {
            res.sendText("Hello world!");
        });
        server.addRequestHandler("/x", HttpRequest::GET, [](HttpRequest &req, HttpResponse &res) {
            
        });



        server.listen(port);
        cout << "Listening on host: " << host
                            << ", port: " << port << "\n";

        // //Stop server
        // cout << "Press any key to stop.\n";
        // cin.get();
        
        server.stop();
    }
    catch(Exception::InternalServerError &err)
    {
        cout << err.what() << "\n";
    }
    
    return 0;
}
