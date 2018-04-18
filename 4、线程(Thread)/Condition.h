//
// Created by 陈希 on 2018/4/9.
//

#ifndef REACTOR_CONDITION_H
#define REACTOR_CONDITION_H

#include "Mutex.h"


class Condition
{
public:
    /*
    * Non-copyable
    */
    Condition(const Condition& other) = delete;
    Condition&operator=(const Condition& other) = delete;
public:
    explicit Condition(Mutex& mutex):
        _mutex(mutex)
    {
        CHECK(pthread_cond_init(&_condition, NULL));
    }
    ~Condition()
    {
        CHECK(pthread_cond_destroy(&_condition));
    }
    void wait()
    {
        CHECK(pthread_cond_wait(&_condition,&(_mutex.getMutex())));
    }
    void notify()
    {
        CHECK(pthread_cond_signal(&_condition));
    }
    void notifyAll()
    {
        CHECK(pthread_cond_broadcast(&_condition));
    }
private:
    Mutex& _mutex;
    pthread_cond_t _condition;
};

#endif //REACTOR_CONDITION_H
