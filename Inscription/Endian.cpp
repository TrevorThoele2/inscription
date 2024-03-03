#include "Endian.h"

namespace Inscription
{
    bool IsLittleEndian()
    {
        static long long n = 1;
        static bool value = reinterpret_cast<char&>(n) == 1;
        return value;
    }

    bool IsBigEndian()
    {
        return !IsLittleEndian();
    }
}