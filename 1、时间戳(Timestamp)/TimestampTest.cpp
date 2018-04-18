//
// Created by 陈希 on 2018/4/14.
//

#include "../Timestamp.h"
#include <vector>

void benchmark()
{
    const int kNumber = 1000*1000;

    std::vector<Timestamp> stamps;
    stamps.reserve(kNumber);
    for (int i = 0; i < kNumber; ++i)
    {
        stamps.push_back(Timestamp::getTimestampNow());
    }
    printf("%s\n", stamps.front().toString().c_str());
    printf("%s\n", stamps.back().toString().c_str());
    printf("%f\n", timeDifference(stamps.back(), stamps.front()));

    int increments[100] = { 0 };
    int64_t start = stamps.front().microSecondsSinceEpoch();
    for (int i = 1; i < kNumber; ++i)
    {
        int64_t next = stamps[i].microSecondsSinceEpoch();
        int64_t inc = next - start;
        start = next;
        if (inc < 0)
        {
            printf("reverse!\n");
        }
        else if (inc < 100)
        {
            ++increments[inc];
        }
        else
        {
            printf("big gap %d\n", static_cast<int>(inc));
        }
    }

    for (int i = 0; i < 100; ++i)
    {
        printf("%2d: %d\n", i, increments[i]);
    }
}

int main()
{
    Timestamp now(Timestamp::getTimestampNow());
    printf("%s\n", now.toString().c_str());
    printf("%s\n", now.toFormattedString(false).c_str());
    printf("%s\n", now.toFormattedString().c_str());
    benchmark();
    return 0;
}