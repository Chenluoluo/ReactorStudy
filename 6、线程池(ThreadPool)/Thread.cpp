//
// Created by 陈希 on 2018/4/16.
//

#include "Thread.h"
#include "CurrentThread.h"
#include "Timestamp.h"
#include <sys/syscall.h>
namespace CurrentThread
{
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "unknown";
}



pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
    CurrentThread::t_cachedTid = 0;
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    // no need to call pthread_atfork(NULL, NULL, &afterFork);
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        CurrentThread::t_threadName = "main";
        CurrentThread::tid();
        pthread_atfork(NULL, NULL, &afterFork);
    }
};

ThreadNameInitializer init;




struct ThreadData
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc _func;
    std::string _name;
    pid_t* _tid;
    CountDownLatch* _latch;

    ThreadData(const ThreadFunc& func, const std::string& name, pid_t* tid, CountDownLatch* latch) :
            _func(func), _name(name), _tid(tid), _latch(latch)
    { }

    void runInThread()
    {
        *_tid = CurrentThread::tid();
        _tid = NULL;
        _latch->countDown();
        _latch = NULL;

        CurrentThread::t_threadName = _name.empty() ? "muduoThread" : _name.c_str();
//        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
        try
        {
            _func();
            CurrentThread::t_threadName = "finished";
        }
        catch (const std::exception& ex)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
            fprintf(stderr, "reason: %s\n", ex.what());
            abort();
        }
        catch (...)
        {
            CurrentThread::t_threadName = "crashed";
            fprintf(stderr, "unknown exception caught in Thread %s\n", _name.c_str());
            throw; // rethrow
        }
    }
};





void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0)
    {
        t_cachedTid = gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainThread()
{
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
    struct timespec ts = { 0, 0 };
    ts.tv_sec = static_cast<time_t>(usec / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
    ::nanosleep(&ts, NULL);
}


AtomicInt32 Thread::_numCreated;

void* startThread(void* obj)
{
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}

Thread::Thread(const Thread::ThreadFunc &func, const std::string &name) :
    _started(false),_jonied(false),_pthreadId(0),_tid(0),_func(std::move(func)),_name(name),_latch(1)
{
    setDefaultName();
}

void Thread::setDefaultName() {
    int num = _numCreated.incrementAndGet();
    if (_name.empty())
    {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        _name = buf;
    }
}

Thread::~Thread()
{
    if (_started && !_jonied)
    {
        pthread_join(_pthreadId, NULL);
    }
}

void Thread::start()
{
    _started = true;
    ThreadData* data = new ThreadData(_func, _name, &_tid, &_latch);
    if (pthread_create(&_pthreadId, NULL, &startThread, data))
    {
        _started = false;
        delete data; // or no delete?
        fprintf(stderr, "Failed in pthread_create");
//        LOG_SYSFATAL << "Failed in pthread_create";
    }
    else
    {
        _latch.wait();
//        assert(_tid > 0);
    }
}

int Thread::join()
{
    _jonied = true;
    return pthread_join(_pthreadId, NULL);
}