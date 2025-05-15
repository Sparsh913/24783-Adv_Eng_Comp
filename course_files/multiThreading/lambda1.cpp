#include <iostream>
#include <string>

int main(void)
{
    auto f=[](std::string str)
    {
        std::cout << str << std::endl;
    };
    f("What is this weird thing called lambda?");
    return 0;
}
