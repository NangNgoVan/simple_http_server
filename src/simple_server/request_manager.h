#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <functional>
#include <thread>
#include <queue>
#include <mutex>

#include "http_request.h"
#include "http_response.h"

typedef std::function<void(HttpRequest&, HttpResponse&)> REQUEST_HANDLER;

class RequestManager {
public:
    explicit RequestManager(int workerNumber = 3);
    ~RequestManager();

    void addTask(std::function<void()> task);
    void doTask();

private:
    std::vector<std::thread> m_workers;

    std::queue<std::function<void()>> m_tasks;

    std::mutex m_mutex;
};

#endif

