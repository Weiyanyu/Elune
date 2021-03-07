#ifndef ELUNE_H
#define ELUNE_H
#include <dlfcn.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>
#include <condition_variable>
#include <mutex>


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

    void waitFileChange() {
        std::unique_lock<std::mutex> lock(m_fileChangeMutex);
        m_fileChangeConditon.wait(lock);
        system(compileCmd());
    }
    
protected:
    virtual const char* getLibraryPath() const = 0;
    virtual std::vector<const char*> getSymbols() const = 0;
    virtual std::vector<const char*> getMonitorPath() const = 0;
    virtual const char* compileCmd() const = 0;

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
        //call once
        std::call_once(m_monitorThreadInitFlag,[this](){
            monitorFileChange();
        });
    }

    void loadSymbols()
    {
        for (const char* symbol : getSymbols()) {
            m_symbols[symbol] = dlsym(m_handle, symbol);
        }
    }

    void close()
    {
        dlclose(m_handle);
    }

    void reload()
    {
        close();
        m_symbols.clear();
        load();
    }

    void monitorFileChange() {
        std::thread t([this](){
            int fd = 0;
            char data[255] = {0};
            
            fd = inotify_init();
            for (std::string&& pathName : getMonitorPath()) {
                inotify_add_watch(fd, pathName.c_str(), IN_CLOSE_WRITE);
            }
            while (true) {
                int num = read(fd, data, 255);
                if (num > 0) {
                    char* p = nullptr; 
                    struct inotify_event* event = nullptr;
                    for (p = data; p < (data + num); ) {    
                        event = (struct inotify_event*)p;
                        {
                            std::lock_guard<std::mutex> lock(m_fileChangeMutex);
                        }
                        //notify
                        m_fileChangeConditon.notify_one();
                        p += sizeof(struct inotify_event) + event->len;
                    }
                }
            }
        });
        t.detach();
    }

    void* m_handle;
    std::unordered_map<std::string, void*> m_symbols;
    std::once_flag m_monitorThreadInitFlag;
    std::condition_variable m_fileChangeConditon;
    std::mutex m_fileChangeMutex;
};

} //namespace elune



#endif