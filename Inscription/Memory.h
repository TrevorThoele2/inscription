#pragma once

namespace Inscription
{
    template<class T>
    T* CreateStorage()
    {
        return reinterpret_cast<T*>(new unsigned char[sizeof(T)]);
    }

    inline void* CreateStorage(size_t memorySize)
    {
        return reinterpret_cast<void*>(new unsigned char[memorySize]);
    }
}