#pragma once

#include <algorithm>

namespace Inscription
{
    bool IsLittleEndian();
    bool IsBigEndian();

    template<class T>
    void SwapBytes(T& t)
    {
        size_t size = sizeof(t);
        if (size >= 2)
        {
            char* byteArray = reinterpret_cast<char*>(&t);

            const auto halfSize = size / 2;
            const auto sizeCompare = --size;
            for (size_t loop = 0; loop < halfSize; ++loop)
                std::swap(byteArray[loop], byteArray[sizeCompare - loop]);
        }
    }

    template<class T>
    void EnsureCorrectEndianness(T& t)
    {
        if (IsLittleEndian())
            return;

        SwapBytes(t);
    }
}