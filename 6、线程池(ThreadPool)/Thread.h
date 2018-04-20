//
// Created by 陈希 on 2018/4/16.
//

#ifndef REACTOR_THREAD_H
#define REACTOR_THREAD_H


#include <functional>
#include "AtomicIntegerT.h"
#include "CountDownLatch.h"

class Thread {
public:
    /*
     * Non-copyable
     */
    Thread(const Thread& other) = delete;
    Thread&operator=(const Thread& other) = delete;

public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
    ~Thread();

    //线程开始
    void start();
    //线程结束
    int join();


    //是否正在运行
    bool started() const { return _started; }
    //是否退出
    bool joined() const { return _jonied; }
    //线程名称
    const std::string& name() const { return _name; }
    //线程的公共id
    pid_t tid() const { return _tid; }
    //线程的私有id
    pthread_t pthreadId() const { return _pthreadId; }
    //创建的线程数
    static int numCreated() { return _numCreated.get(); }

private:
    void setDefaultName();

    bool _started;
    bool _jonied;
    pthread_t _pthreadId;
    pid_t _tid;
    ThreadFunc _func;
    std::string _name;
    CountDownLatch _latch;

    static AtomicInt32 _numCreated;
};


#endif //REACTOR_THREAD_H
