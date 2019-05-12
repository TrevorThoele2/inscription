#pragma once

#include <algorithm>

namespace Inscription
{
    bool IsLittleEndian();
    bool IsBigEndian();

    namespace detail
    {
        template<class T>
        void ByteSwapImplementation(T& t)
        {
            size_t size = sizeof(t);
            if (size < 2)
                return;

            char* byteArray = reinterpret_cast<char*>(&t);

            const auto halfSize = size / 2;
            const auto sizeCompare = --size;
            for (size_t loop = 0; loop < halfSize; ++loop)
                std::swap(byteArray[loop], byteArray[sizeCompare - loop]);
        }
    }

    template<class T>
    void ByteSwap(T& t)
    {
        if (IsLittleEndian())
            return;

        detail::ByteSwapImplementation(t);
    }
}