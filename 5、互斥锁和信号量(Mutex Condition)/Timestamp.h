//
// Created by 陈希 on 2018/4/14.
//

#ifndef REACTOR_TIMESTAMP_H
#define REACTOR_TIMESTAMP_H


#include <stdint.h>
#include <string>


class Timestamp {
public:
    Timestamp()
            : _microSecondsSinceEpoch(0)
    {
    }
    explicit Timestamp(int64_t microSecondsSinceEpochArg)
            : _microSecondsSinceEpoch(microSecondsSinceEpochArg)
    {
    }

    //判断是否大于0
    bool valid() const { return _microSecondsSinceEpoch > 0; }

    //直接返回时间(秒+微秒)
    int64_t microSecondsSinceEpoch() const { return _microSecondsSinceEpoch; }
    //返回时间(秒)
    time_t secondsSinceEpoch() const { return static_cast<time_t>(_microSecondsSinceEpoch / kMicroSecondsPerSecond); }

    //转换成字符串
    std::string toString() const;
    //转换成格式化字符串
    std::string toFormattedString(bool showMicroseconds = true) const;



    //获得当前时间戳
    static Timestamp getTimestampNow();
    //获得0时间戳
    static Timestamp getTimestampZero()
    {
        return Timestamp();
    }
    //获得时间戳(秒)
    static Timestamp getTimestamp(time_t t)
    {
        return getTimestamp(t, 0);
    }
    //获得时间戳(秒+微秒)
    static Timestamp getTimestamp(time_t t, int microseconds)
    {
        return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
    }

    //定义微秒和秒的换算
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    //时间
    int64_t _microSecondsSinceEpoch;
};

//重载判断操作符

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator<=(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() <= rhs.microSecondsSinceEpoch();
}
inline bool operator>(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}
inline bool operator>=(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() >= rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

//返回相差的秒数
inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}
//返回增加秒数的时间戳
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}


#endif //REACTOR_TIMESTAMP_H
