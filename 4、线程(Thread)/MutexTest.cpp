//
// Created by 陈希 on 2018/4/17.
//

#include <vector>
#include <printf.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include "Mutex.h"
#include "Timestamp.h"
#include "Thread.h"

Mutex g_mutex;
std::vector<int> g_vec;
//AtomicInt64 atomic;
const int kCount = 10*1000*1000;

void threadFunc()
{
    for (int i = 0; i < kCount; ++i)
    {
        MutexGuard lock(g_mutex);
        g_vec.push_back(i);
//        atomic.increment();
    }
}

int foo() __attribute__ ((noinline));

int g_count = 0;
int foo()
{
    MutexGuard lock(g_mutex);
    if (!g_mutex.isLockedByThisThread())
    {
        printf("FAIL\n");
        return -1;
    }
    ++g_count;
    return 0;
}

int main()
{

    int ret = foo();
    assert(ret == 0);
    if (g_count != 1)
    {
        printf("MCHECK calls twice.\n");
        abort();
    }

    const int kMaxThreads = 8;
    g_vec.reserve(kMaxThreads * kCount);

    Timestamp start(Timestamp::getTimestampNow());
    for (int i = 0; i < kCount; ++i)
    {
        g_vec.push_back(i);
    }

    printf("single thread without lock %f\n", timeDifference(Timestamp::getTimestampNow(), start));

    start = Timestamp::getTimestampNow();
    threadFunc();
    printf("single thread with lock %f\n", timeDifference(Timestamp::getTimestampNow(), start));
    boost::ptr_vector<Thread> threads;
    for (int nthreads = 1; nthreads < kMaxThreads; ++nthreads)
    {
        g_vec.clear();
        start = Timestamp::getTimestampNow();
        for (int i = 0; i < nthreads; ++i)
        {
            threads.push_back(new Thread(&threadFunc));
            threads.back().start();
        }
        for (int i = 0; i < nthreads; ++i)
        {
            threads[i].join();
        }
        printf("%d thread(s) with lock %f\n", nthreads, timeDifference(Timestamp::getTimestampNow(), start));
    }
    printf("Threads : %d \n", Thread::numCreated());
    return 0;
}
