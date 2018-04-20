//
// Created by 陈希 on 2018/4/18.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(const std::string &nameArg)
    : _mutex(),
      _notEmpty(_mutex),
      _notFull(_mutex),
      _name(nameArg),
      _maxQueueSize(0),
      _running(1)
{

}

ThreadPool::~ThreadPool()
{
    if (_running)
        stop();
}

void ThreadPool::start(int numThreads)
{
    assert(_threads.empty());
    _running = true;
    _threads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i) {
        char id[32];
        snprintf(id, sizeof id, "%d", i+1);
        _threads.push_back(new Thread(
                std::bind(&ThreadPool::runInThread, this), _name+id));
        _threads[i].start();
    }
    if (numThreads == 0 && _threadInitCallback)
    {
        _threadInitCallback();
    }
}

void ThreadPool::stop()
{
    {
        MutexGuard lock(_mutex);
        _running = false;
        _notEmpty.notifyAll();
    }
    for (int i = 0; i < _threads.size(); ++i)
    {
        _threads[i].join();
    }
//    std::for_each(_threads.begin(), _threads.end(), std::bind(&Thread::join, _1));
}

void ThreadPool::run(const ThreadPool::Task &task)
{
    if (_threads.empty())
    {
        task();
    }
    else
    {
        MutexGuard lock(_mutex);
        while (isFull())
        {
            _notFull.wait();
        }
        assert(!isFull());
        _queue.push_back(task);
        _notEmpty.notify();
    }
}

bool ThreadPool::isFull() const {
//    _mutex.assertLocked();
    return _maxQueueSize > 0 && _queue.size() >= _maxQueueSize;

}

void ThreadPool::runInThread()
{
    try
    {
        if (_threadInitCallback)
        {
            _threadInitCallback();
        }
        while (_running)
        {
            Task task(take());
            if (task)
            {
                task();
            }
        }
    }
    catch (const std::exception& ex)
    {
        fprintf(stderr, "exception caught in ThreadPool %s\n", _name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
    }
    catch (...)
    {
        fprintf(stderr, "unknown exception caught in ThreadPool %s\n", _name.c_str());
        throw;
    }

}

ThreadPool::Task ThreadPool::take() {
    MutexGuard lock(_mutex);
    while (_queue.empty() && _running)
    {
        _notEmpty.wait();
    }
    Task task;
    if (!_queue.empty())
    {
        task = _queue.front();
        _queue.pop_front();
        if (_maxQueueSize > 0)
        {
            _notFull.notify();
        }
    }
    return task;
}
