//
// Created by 陈希 on 2018/4/16.
//

#ifndef REACTOR_SINGLETON_H
#define REACTOR_SINGLETON_H

#include <pthread.h>

template<typename T>
class Singleton
{

public:
    /*
     * Non-copyable
     */
    Singleton(const Singleton& other) = delete;
    Singleton&operator=(const Singleton& other) = delete;
public:
    static T& instance()
    {
        pthread_once(&_ponce, &Singleton::init);
        return *_value;
    }

private:
    Singleton();
    ~Singleton();
    static void init()
    {
        _value = new T();
    }

    static pthread_once_t _ponce;
    static T* _value;
};

template<typename T>
pthread_once_t Singleton<T>::_ponce = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::_value = NULL;

#endif //REACTOR_SINGLETON_H
