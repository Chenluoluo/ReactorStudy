//
// Created by 陈希 on 2018/4/9.
//

#ifndef REACTOR_MUTEX_H
#define REACTOR_MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "CurrentThread.h"

#define CHECK(ret) ({__typeof__(ret) err=(ret);assert(err==0);})

class Mutex
{
public:
    /*
     * Non-copyable
     */
    Mutex(const Mutex& other) = delete;
    Mutex&operator=(const Mutex& other) = delete;
public:
    Mutex()
        : _holder(0)
    {
        CHECK(pthread_mutex_init(&_mutex, NULL));
    }
    ~Mutex()
    {
        assert(_holder == 0);
        CHECK(pthread_mutex_destroy(&_mutex));
    }

    bool isLockedByThisThread() const
    {
        return _holder == CurrentThread::tid();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        CHECK(pthread_mutex_lock(&_mutex));
        assignHolder();
    }
    void unlock()
    {
        CHECK(pthread_mutex_unlock(&_mutex));
        unassignHolder();
    }
    pthread_mutex_t& getMutex()
    {
        return _mutex;
    }
private:

    void unassignHolder()
    {
        _holder = 0;
    }

    void assignHolder()
    {
        _holder = CurrentThread::tid();
    }

    friend class Condition;

    class UnassignGuard
    {
    public:
        UnassignGuard(Mutex& owner)
                : owner_(owner)
        {
            owner_.unassignHolder();
        }

        ~UnassignGuard()
        {
            owner_.assignHolder();
        }

    private:
        Mutex& owner_;
    };

    pthread_mutex_t _mutex;
    pid_t _holder;
};



class MutexGuard{
public:
    /*
    * Non-copyable
    */
    MutexGuard(const MutexGuard& other) = delete;
    MutexGuard&operator=(const MutexGuard& other) = delete;
public:
    explicit MutexGuard(Mutex& mutex):
            _mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex& _mutex;
};


#endif //REACTOR_MUTEX_H
