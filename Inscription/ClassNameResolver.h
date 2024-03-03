#pragma once

#include <functional>

#include "ClassName.h"

namespace Inscription
{
    class Scribe;
    class ClassNameResolver
    {
    public:
        template<class Func>
        ClassNameResolver(Func func);

        ClassName NameFor(Scribe &scribe) const;
    private:
        typedef std::function<ClassName(Scribe&)> Underlying;
        Underlying underlying;
    };

    template<class Func>
    ClassNameResolver::ClassNameResolver(Func func) : underlying(func)
    {}
}