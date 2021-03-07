#include "test.h"
#include <iostream>
#include <signal.h>

int main()
{
    TestModel& model = TestModel::getInstance();
    model.loadLibrary();

    while (true) {
        model.foo();
        std::cout << model.getBar() << std::endl;
        model.waitFileChange();
        model.reloadLibrary();
    }

    return 0;
}