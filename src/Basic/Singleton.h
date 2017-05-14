#pragma once

template <class T> class Singleton
{
public:
    static T* Instance()
    {
        static T* instance = new T();
        return instance;
    }

protected:
    // Can't instantiate apart from Instance method
    Singleton() = default;
};
