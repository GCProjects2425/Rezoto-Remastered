#pragma once
#include <mutex>

template < typename T >
class Singleton {
public:
    static T* GetInstance() {
        static MemGuard g;
        std::lock_guard<std::mutex> lock(mutex_);
        if (!m_instance) {
            m_instance = new T();
        }
        return m_instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {};
    virtual ~Singleton() {}

private:
    inline static T* m_instance = nullptr;
    inline static std::mutex mutex_;

    class MemGuard {
    public:
        ~MemGuard() {
            delete m_instance;
            m_instance = nullptr;
        }
    };
};