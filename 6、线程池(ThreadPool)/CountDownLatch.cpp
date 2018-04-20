//
// Created by 陈希 on 2018/4/12.
//

#include "CountDownLatch.h"

CountDownLatch::CountDownLatch(int count)
        : _mutex(),
          _condition(_mutex),
          _count(count)
{
}

void CountDownLatch::wait()
{
    MutexGuard lock(_mutex);
    while (_count > 0)
    {
        _condition.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexGuard lock(_mutex);
    --_count;
    if (_count == 0)
    {
        _condition.notifyAll();
    }
}

int CountDownLatch::getCount() const
{
    MutexGuard lock(_mutex);
    return _count;
}