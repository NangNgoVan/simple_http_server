#include "simple_server.h"

SimpleServer::SimpleServer(string host) : m_sockFd(-1),
                                          m_host(host),
                                          m_isRunning(false),
                                          m_maxConnectionInQueue(10000)
{
    // create a new socket
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
}

SimpleServer::~SimpleServer()
{
    stop();
}

std::string SimpleServer::getHost()
{
    return m_host;
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
                   sizeof(opt)) < 0)
    {
        close(m_sockFd);
        throw Exception::InternalServerError("Socket reused error!");
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

    if (::listen(m_sockFd, m_maxConnectionInQueue) < 0)
    {
        // Listen error
        close(m_sockFd);
        throw Exception::InternalServerError("Cannot listen on port!");
    }

    cout << "Server listening: " << m_maxConnectionInQueue << " connections.\n";

    m_isRunning = true;

    

    struct sockaddr_in c_addr;
    int addrlen = sizeof(c_addr);

    while (1)
    {
        int c_socket = ::accept(m_sockFd, (sockaddr *)&c_addr, (socklen_t *)&addrlen);

        // if (c_socket < 0)
        // {
        //     close(c_socket);
        // }

        // char buffer[1024] = {0};

        // int valread = read(c_socket, buffer, 1024);

        // if (valread < 0)
        // {
        //     perror("Error when read buffer\n");
        //     close(c_socket);
        // }
        // bzero(buffer, 1024);


        // std::cout << "add task\n";
        handleRequest(c_socket);

        // m_manager.addTask([this, &c_socket](){
        //     std::cout << "socket: " << c_socket << "\n";
        //     handleRequest(c_socket);
        // });
    }
}

void SimpleServer::stop()
{
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

void SimpleServer::handleRequest(const int &clientSocket)
{
    if (clientSocket < 0)
    {
        close(clientSocket);
    }

    char buffer[1024] = {0};

    int valread = read(clientSocket, buffer, 1024);

    if (valread < 0)
    {
        perror("Error when read buffer\n");
        close(clientSocket);
    }

    try
    {
        HttpRequest req;
        req.parse(buffer);

        REQUEST_HANDLER *handler = getRequestHandler(req.getPath());
        HttpResponse res;

        if (handler)
        {
            
            res.setStatusCode(HttpResponse::OK);

            (*handler)(req, res);

            std::string responseResult = res.toString();

            int sended = send(clientSocket, responseResult.c_str(), responseResult.size(), 0);

            if (sended < 0) {
                perror("Send data error!");
            }
        }
        else
        {
            // perror("Request handler not found\n");
        }
    }
    catch (Exception::HttpMessageParseError &err)
    {
        perror("Http parse message error!\n");
    }

    bzero(buffer, 1024);
    close(clientSocket);
}
