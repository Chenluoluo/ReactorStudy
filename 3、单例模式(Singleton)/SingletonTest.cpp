//
// Created by 陈希 on 2018/4/16.
//

#include "../Singleton.h"
#include <stdio.h>

class Test
{
public:
    Test(): _num(0)
    {
        printf("Test()\n");
    }
    ~Test()
    {
        printf("~Test()\n");
    }
    void Print()
    {
        printf("num:%d\n",_num);
    }
    int _num;
};

int main()
{
    Test& test = Singleton<Test>::instance();
    test.Print();
    Test& test2 = Singleton<Test>::instance();
    test2.Print();
    test._num++;
    test.Print();

    printf("%x\n",test);
    printf("%x\n",test2);
    return 0;

}