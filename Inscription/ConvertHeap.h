#pragma once

namespace Inscription
{
    template<class Original, class Converted>
    Converted* ConvertHeap(const Original& orig)
    {
        return new Converted(orig);
    }
}