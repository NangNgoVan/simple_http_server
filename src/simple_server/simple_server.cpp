#include "simple_server.h"

SimpleServer::SimpleServer(string host, int maxThread) : m_sockFd(-1),
                                                         m_host(host),
                                                         m_isRunning(false),
                                                         m_maxConnectionInQueue(10)
{
    m_epollCFds.resize(maxThread);
    // create a new socket
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    // create epolls
    m_epollFd = epoll_create1(0);
    if (m_epollFd < 0)
    {
        throw Exception::InternalServerError("Cannot create server epoll!");
    }

    for (int i = 0; i < m_epollCFds.size(); i++)
    {
        m_epollCFds[i] = epoll_create1(0);

        if (m_epollCFds[i] < 0)
        {
            throw Exception::InternalServerError("Cannot create client epoll!");
        }
    }
}

void SimpleServer::handleServerEpoll()
{
    epoll_event events[10];

    int nfds;

    while (1)
    {
        nfds = epoll_wait(m_epollFd, events, 10, -1);
        if (nfds < 0)
        {
            throw Exception::InternalServerError("Epoll wait event error!");
        }

        int currentEpollIdx = 0;

        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == m_sockFd)
            {
                struct sockaddr_in c_addr;
                int addrlen;
                int c_socket;

                addrlen = sizeof(c_addr);
                c_socket = ::accept(m_sockFd, (sockaddr *)&c_addr, (socklen_t *)&addrlen);
                if (c_socket < 0)
                {
                    break;
                }

                if (fcntl(c_socket, F_SETFL, O_NONBLOCK) < 0)
                {
                    close(c_socket);
                    throw Exception::InternalServerError("Cannot set non-blocking mode of the client connection");
                }

                epoll_event ev;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = c_socket;

                ;
                currentEpollIdx = (++currentEpollIdx) % m_epollCFds.size();
                // std::cout << "Add to epoll: " << m_currentEpollIdx << "\n";

                if (epoll_ctl(m_epollCFds[currentEpollIdx], EPOLL_CTL_ADD, c_socket, &ev) < 0)
                {
                    throw Exception::InternalServerError("Cannot add client Fd to epoll!");
                }
            }
        }
    }
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

    // non-blocking mode
    if (fcntl(m_sockFd, F_SETFL, O_NONBLOCK) < 0)
    {
        close(m_sockFd);
        throw Exception::InternalServerError("Cannot set non-blocking mode");
    }

    if (::listen(m_sockFd, m_maxConnectionInQueue) < 0)
    {
        // Listen error
        close(m_sockFd);
        throw Exception::InternalServerError("Cannot listen on port!");
    }

    m_isRunning = true;

    struct sockaddr_in c_addr;
    int addrlen;
    int c_socket;

    for (int i = 0; i < m_epollCFds.size(); i++)
    {
        m_threads.push_back(std::thread(&SimpleServer::handleClientEpoll, this, i));
    }

    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = m_sockFd;

    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_sockFd, &ev) < 0)
    {
        throw Exception::InternalServerError("Cannot add server Fd to epoll");
    }

    // std::thread t(&SimpleServer::handleServerEpoll, this);
    handleServerEpoll();
}

void SimpleServer::stop()
{
    m_isRunning = false;

    close(m_sockFd);
    close(m_epollFd);
    for (int i = 0; i < m_epollCFds.size(); i++)
        close(m_epollCFds[i]);
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

void SimpleServer::handleRequest(int clientSocket)
{
    char buffer[2048] = {0};

    int valread = read(clientSocket, buffer, 2048);

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

            int sended = write(clientSocket, responseResult.c_str(), responseResult.size());

            if (sended < 0)
            {
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

void SimpleServer::handleClientEpoll(int epollIdx)
{
    struct epoll_event events[10];

    int nfds;

    // std::cout << "thread:" << std::this_thread::get_id() << "\n";

    while (1)
    {
        nfds = epoll_wait(m_epollCFds[epollIdx], events, 10, -1);

        if (nfds < 0)
        {
            throw Exception::InternalServerError("Epoll client wait event error!");
        }

        for (int i = 0; i < nfds; ++i)
        {
            char buffer[1024] = {0};

            int valread = read(events[i].data.fd, buffer, 1024);

            if (valread < 0)
            {
                if (valread == 0)
                    perror("Error when read buffer\n");
                epoll_ctl(m_epollCFds[epollIdx], EPOLL_CTL_DEL, events[i].data.fd, NULL);
                close(events[i].data.fd);
                break;
            }

            // std::cout << "Buffer:" << buffer << "\n";
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

                    int sended = write(events[i].data.fd, responseResult.c_str(), responseResult.size());

                    if (sended < 0)
                    {
                        perror("Send data error!");
                    }

                    // std::cout << "thread:" << std::this_thread::get_id() << ", id: "<< epollIdx << " send!\n";
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

            epoll_ctl(m_epollCFds[epollIdx], EPOLL_CTL_DEL, events[i].data.fd, NULL);
            close(events[i].data.fd);
        }
    }
}
