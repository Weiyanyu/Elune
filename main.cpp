#include "test.h"
#include <iostream>

int main()
{
    // TestModel model;
    TestModel::loadLibrary();
    TestModel::foo();
    std::cout << TestModel::getBar() << std::endl;

    return 0;
}