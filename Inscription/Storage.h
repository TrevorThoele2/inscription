#pragma once

namespace Inscription
{
    inline std::uint8_t* CreateStorage(size_t size)
    {
        return new std::uint8_t[size];
    }

    inline void DestroyStorage(std::uint8_t* storage)
    {
        delete[] storage;
    }
}