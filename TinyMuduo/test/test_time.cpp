#include <iostream>
#include "../base/Timestamp.h"
int main()
{
    std::cout << Timestamp::now().toString() << std::endl;
    return 0;
}