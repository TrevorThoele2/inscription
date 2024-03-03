#pragma once

#include <functional>

#include "ClassName.h"

namespace Inscription
{
    template<class Archive>
    class ClassNameResolver
    {
    public:
        using ArchiveT = Archive;
    public:
        template<class Function>
        ClassNameResolver(Function function);

        ClassName NameFor(ArchiveT& archive) const;
    private:
        using RetrieveClassName = std::function<ClassName(ArchiveT&)>;
        RetrieveClassName retriever;
    };

    template<class Archive>
    template<class Function>
    ClassNameResolver<Archive>::ClassNameResolver(Function function) : retriever(function)
    {}

    template<class Archive>
    ClassName ClassNameResolver<Archive>::NameFor(ArchiveT& archive) const
    {
        return retriever(archive);
    }
}