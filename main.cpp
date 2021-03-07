#include "test.h"
#include <iostream>

int main()
{
    // TestModel model;
    TestModel::loadLibrary();
    TestModel::foo();
    std::cout << TestModel::getBar() << std::endl;

    while (std::cin.get() != '\n') {}
    TestModel::reloadLibrary();
    TestModel::foo();
    std::cout << TestModel::getBar() << std::endl;

    return 0;
}