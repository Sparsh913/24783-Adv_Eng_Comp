#include <iostream>
#include <string>
#include <unordered_set>
int main(void)
{
    std::hash <std::string> func;
    std::cout << func("xyz") << std::endl;
    std::cout << func("zyx") << std::endl;
}
