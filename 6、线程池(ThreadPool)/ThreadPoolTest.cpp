//
// Created by 陈希 on 2018/4/18.
//

#include <stdio.h>
#include <iostream>
#include "../CurrentThread.h"
#include "../ThreadPool.h"

using namespace std;

void print()
{
    printf("tid=%d\n", CurrentThread::tid());
}

void printString(const std::string& str)
{
    cout << str+"\n";
    usleep(100*1000);
}

void test(int maxSize)
{
    cout << "Test ThreadPool with max queue size = " << maxSize;
    ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    cout << "Adding\n";
    pool.run(print);
    pool.run(print);
    for (int i = 0; i < 100; ++i)
    {
        char buf[32];
        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(std::bind(printString, std::string(buf)));
    }
    cout << "Done\n";

    CountDownLatch latch(1);
    pool.run(std::bind(&CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}
