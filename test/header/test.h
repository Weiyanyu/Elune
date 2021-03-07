#ifndef TEST_H
#define TEST_H
#include <Elune.hpp>

extern "C"
{
    void foo();
    extern int bar;
}

constexpr const char* LIBRARY_PATH = "/home/weiyanyu/learn/cpp/Elune/bin/libtest.so";

class TestModel : public elune::EluneModel<TestModel>
{
public:

    static void foo() 
    {
        getInstance().call<void>("foo");
    }

    static int getBar() 
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
private:
    std::vector<const char*> m_exportSymbol = {
        "foo",
        "bar"   
    };
};

#endif