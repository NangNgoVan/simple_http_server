#include "request_manager.h"

RequestManager::RequestManager(int workerNumber)
{
    std::cout << "Create request manager.\n";

    for (int i = 0; i < workerNumber; i++) {
        m_workers.push_back(std::thread(&RequestManager::doTask, this));
    }
}

RequestManager::~RequestManager()
{
    for (auto & worker : m_workers) 
    {
        worker.join();
    }
}

void RequestManager::doTask()
{
    while (true)
    {
        bool found = false;
        std::function<void()> task;
        m_mutex.lock();
        if (!m_tasks.empty())
        {
            found = true;
            task = m_tasks.front();
            m_tasks.pop();
        }
        m_mutex.unlock();
        if (found) task(); //do job
    }
}

void RequestManager::addTask(std::function<void()> task)
{
    m_tasks.push(task);
}