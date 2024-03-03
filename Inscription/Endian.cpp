
#include "Endian.h"

namespace inscription
{
    bool IsLittleEndian()
    {
        static long long n = 1;
        static bool value = *(char *)&n == 1;
        return value;
    }
}