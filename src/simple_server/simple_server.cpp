#include "simple_server.h"

SimpleServer::SimpleServer(string host) : m_sockFd(-1),
                                          m_host(host),
                                          m_isRunning(false)
{
    // create a new socket
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
}

SimpleServer::~SimpleServer()
{
    // stop();
}

void SimpleServer::listen(
    int port)
{

    if (m_sockFd < 0)
    {
        throw Exception::InternalServerError("Cannot create socket");
    }

    int opt = 1;

    if (setsockopt(m_sockFd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {

        // throw Exception::InternalServerError("Socket option error!");
    }

    struct sockaddr_in s_addr;

    bzero((char *)&s_addr, sizeof(s_addr));

    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    // inet_pton(AF_INET, m_host.c_str(), &(s_addr.sin_addr.s_addr));
    s_addr.sin_port = htons(port);

    if (::bind(m_sockFd, (sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
        // Bind socket error
        close(m_sockFd);
        throw Exception::InternalServerError("Bind socket error");
    }

    // Maximum of backlog
    int maximumBacklog = 5;

    if (::listen(m_sockFd, maximumBacklog) < 0)
    {
        // Listen error
        close(m_sockFd);
        throw Exception::InternalServerError("Cannot listen on port!");
    }

    m_isRunning = true;

    while (1)
    {
        waitForClientConnection();
    }
}

void SimpleServer::waitForClientConnection()
{
    //
    struct sockaddr_in c_addr;
    int addrlen = sizeof(c_addr);

    int c_socket = ::accept(m_sockFd, (sockaddr *)&c_addr, (socklen_t *)&addrlen);

    if (c_socket < 0)
    {
        // Error
        close(c_socket);
    }

    // cout << "New connection commning\n";
    char buffer[1024] = {0};

    int valread = read(c_socket, buffer, 1024);

    if (valread < 0)
    {
        // Error
        cout << "Error when read buffer";
        close(c_socket);
    }

    cout << buffer << "\n";

    HttpRequest req;

    try
    {
        req.parse(buffer);

        REQUEST_HANDLER *handler = getRequestHandler(req.getPath());
        HttpResponse res;

        if (handler)
        {
            

            res.prepareStatusLine(req.getVersion(), HttpMessage::OK);

            (*handler)(req, res);

            //
            res.setHeader("Content-Type", "text/html");

            std::string responseResult = res.toString();
            send(c_socket, responseResult.c_str(), sizeof(responseResult.c_str()), 0);
        }

        else
        {
            // not found
            // res.prepareStatusLine(req.getVersion(), HttpMessage::INTER)
            // throw 
        }
    }
    catch (Exception::HttpMessageParseError &err)
    {
        // parse error
        cout << "Parse error";
    }

    bzero(buffer, 1024);

    close(c_socket);
}

void SimpleServer::stop()
{
    cout << "Stop simple server\n";
    close(m_sockFd);
}

void SimpleServer::addRequestHandler(std::string path,
                                     HttpRequest::HttpMethod method,
                                     std::function<void(HttpRequest &, HttpResponse &)> handler)
{
    m_handlerMap.emplace(path, handler);
};

REQUEST_HANDLER *SimpleServer::getRequestHandler(std::string path)
{
    if (m_handlerMap.find(path) == m_handlerMap.end())
        return 0;

    return &m_handlerMap[path];
};
