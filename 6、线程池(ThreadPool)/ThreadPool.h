//
// Created by 陈希 on 2018/4/18.
//

#ifndef REACTOR_THREADPOOL_H
#define REACTOR_THREADPOOL_H


#include <functional>
#include <boost/ptr_container/ptr_vector.hpp>
#include <deque>
#include "Thread.h"

class ThreadPool {
public:
    /*
     * Non-copyable
     */
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool&operator=(const ThreadPool& other) = delete;

public:
    typedef std::function<void()> Task;

    explicit ThreadPool(const std::string& nameArg = std::string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { _maxQueueSize = maxSize; }
    void setThreadInitCallback(const Task& cb) { _threadInitCallback = cb; }

    void start(int numThreads);
    void stop();

    const std::string& name() const { return _name; }

    size_t queueSize() const
    {
        MutexGuard lock(_mutex);
        return _queue.size();
    }

    void run(const Task& task);

private:
    bool isFull() const;
    void runInThread();
    Task take();


    mutable Mutex _mutex;
    Condition _notEmpty;
    Condition _notFull;
    std::string _name;
    Task _threadInitCallback;
    boost::ptr_vector<Thread> _threads;
    std::deque<Task> _queue;
    size_t _maxQueueSize;
    bool _running;
};


#endif //REACTOR_THREADPOOL_H
