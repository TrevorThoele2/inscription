#pragma once

namespace Inscription
{
    template<class Original, class Converted>
    Converted Convert(const Original &orig)
    {
        return Converted(orig);
    }
}