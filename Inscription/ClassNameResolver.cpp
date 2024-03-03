#include "ClassNameResolver.h"

#include "Scribe.h"

namespace Inscription
{
    ClassName ClassNameResolver::NameFor(Scribe& scribe) const
    {
        return underlying(scribe);
    }
}