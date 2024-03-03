#pragma once

#include <functional>

#include "ClassName.h"

namespace Inscription
{
    template<class ScribeT>
    class ClassNameResolver
    {
    public:
        template<class Function>
        ClassNameResolver(Function function);

        ClassName NameFor(ScribeT& scribe) const;
    private:
        typedef std::function<ClassName(ScribeT&)> RetrieveClassName;
        RetrieveClassName retriever;
    };

    template<class ScribeT>
    template<class Function>
    ClassNameResolver<ScribeT>::ClassNameResolver(Function function) : retriever(function)
    {}

    template<class ScribeT>
    ClassName ClassNameResolver<ScribeT>::NameFor(ScribeT& scribe) const
    {
        return retriever(scribe);
    }
}