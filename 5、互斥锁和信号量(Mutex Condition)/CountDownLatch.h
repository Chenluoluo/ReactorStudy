//
// Created by 陈希 on 2018/4/12.
//

#ifndef REACTOR_COUNTDOWNLATCH_H
#define REACTOR_COUNTDOWNLATCH_H


#include "Condition.h"

class CountDownLatch
{
public:
    /*
    * Non-copyable
    */
    CountDownLatch(const CountDownLatch& other) = delete;
    CountDownLatch&operator=(const CountDownLatch& other) = delete;
public:

    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable Mutex _mutex;
    Condition _condition;
    int _count;
};


#endif //REACTOR_COUNTDOWNLATCH_H
