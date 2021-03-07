#ifndef ELUNE_H
#define ELUNE_H
#include <dlfcn.h>
#include <unordered_map>
#include <vector>
#include <iostream>


namespace elune {

template<typename T>
class EluneModel
{
public:
    //Note: must reference
    static T& getInstance()
    {
        static T instance;
        return instance;
    }

    static void loadLibrary() { getInstance().load(); }
    static void reloadLibrary() { getInstance().reload(); }
    static void closeLibrary() { getInstance().close(); }

protected:
    virtual const char* getLibraryPath() const = 0;
    virtual std::vector<const char*> getSymbols() const = 0;

    template<typename Ret, typename... Args>
    Ret call(const char* functionName, Args... args)
    {
        auto iter = m_symbols.find(functionName);
        if (iter != m_symbols.end()) {
            return reinterpret_cast<Ret(*)(Args...)>(iter->second)(args...);
        } else {
            throw std::runtime_error(std::string("Function not found: ") + functionName);
        }
    }

    template<typename E>
    E* getVariable(const char* variableName)
    {
        auto iter = m_symbols.find(variableName);
        if (iter != m_symbols.end()) {
            return reinterpret_cast<E*>(iter->second);
        } else {
            return nullptr;
        }
    }

private:
    void load()
    {
        m_handle = dlopen(getLibraryPath(), RTLD_NOW);
        loadSymbols();
    }

    void loadSymbols()
    {
        for (const char* symbol : getSymbols()) {
            m_symbols[symbol] = dlsym(m_handle, symbol);
        }
    }

    void close()
    {
        dlclose(loadSymbols);
    }

    void reload()
    {
        close();
        m_symbols.clear();
        load();
    }


    void* m_handle;
    std::unordered_map<std::string, void*> m_symbols;
};

} //namespace elune



#endif