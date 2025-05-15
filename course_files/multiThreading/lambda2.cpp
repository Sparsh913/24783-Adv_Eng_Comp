#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <functional>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    std::function <void()> f[4]=
    {
        [=]()
        {
            std::cout << "function 0" << std::endl;
        },
        [=]()
        {
            std::cout << "function 1" << std::endl;
        },
        [=]()
        {
            std::cout << "function 2" << std::endl;
        },
        [=]()
        {
            std::cout << "function 3" << std::endl;
        }
    };
    srand(time(nullptr));
    f[rand()%4]();
    return 0;
}
