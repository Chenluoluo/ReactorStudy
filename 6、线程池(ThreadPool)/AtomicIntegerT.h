//
// Created by 陈希 on 2018/4/14.
//

#ifndef REACTOR_ATOMICINTEGERT_H
#define REACTOR_ATOMICINTEGERT_H

#include <unitypes.h>

template <typename T>

class AtomicIntegerT {
public:
    /*
    * Non-copyable
    */
    AtomicIntegerT(const AtomicIntegerT& other) = delete;
    AtomicIntegerT&operator=(const AtomicIntegerT& other) = delete;
public:
    AtomicIntegerT() :
        _value(0)
    {
    }

    T get()
    {
        return __atomic_load_n(&_value, __ATOMIC_SEQ_CST);
//        return __sync_val_compare_and_swap(&_value, 0, 0);
    }

    T getAndAdd(T x)
    {
        return __atomic_fetch_add(&_value, x, __ATOMIC_SEQ_CST);
//        return __sync_fetch_and_add(&_value, x);
    }

    T addAndGet(T x)
    {
        return getAndAdd(x) + x;
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    T decrementAndGet()
    {
        return addAndGet(-1);
    }

    void add(T x)
    {
        getAndAdd(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        decrementAndGet();
    }

    T getAndSet(T newValue)
    {
        return __atomic_exchange_n(&_value, newValue, __ATOMIC_SEQ_CST);
//        return __sync_lock_test_and_set(&_value, newValue);
    }
private:
    volatile T _value;
};
typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

#endif //REACTOR_ATOMICINTEGERT_H
