#include "./../include/master.h"
#include <iostream>

int main()
{
    Master master;
    std::cout << "----Slighttpd----" << std::endl;
    if (!master.StartMaster())
        return -1;
    std::cout << "----Goodbye----" << std::endl;
    return 0;
}
