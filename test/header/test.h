#ifndef TEST_H
#define TEST_H
#include <Elune.hpp>
#include <unistd.h>

extern "C"
{
    void foo();
    extern int bar;
}

constexpr const char* LIBRARY_PATH = "/home/weiyanyu/learn/cpp/Elune/bin/libtest.so";
constexpr const char* COMPILE_CMD = "cd /home/weiyanyu/learn/cpp/Elune/test/build;make";

class TestModel : public elune::EluneModel<TestModel>
{ 
public:

    void foo() 
    {
        getInstance().call<void>("foo");
    }

    int getBar() 
    {
        return *(getInstance().getVariable<decltype(bar)>("bar"));
    } 
protected:
    const char* getLibraryPath() const override
    {
        return LIBRARY_PATH;
    }
    std::vector<const char*> getSymbols() const override
    {
        return m_exportSymbol;
    }

    std::vector<const char*> getMonitorPath() const override
    {
        return m_monitorPath;
    }

    const char* compileCmd() const override
    {
        return COMPILE_CMD;
    }
private:
    std::vector<const char*> m_exportSymbol = {
        "foo",
        "bar" 
    };

    std::vector<const char*> m_monitorPath = {
        "/home/weiyanyu/learn/cpp/Elune/test/header",
        "/home/weiyanyu/learn/cpp/Elune/test/src"
    };
};

#endif