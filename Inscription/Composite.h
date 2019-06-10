#pragma once

#include "Scribe.h"
#include "ScribeBase.h"

#include "BaseScriven.h"
#include "ClassNameResolver.h"

namespace Inscription
{
    template<class Object, class Archive>
    class CompositeScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = typename ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    protected:
        using ClassNameResolver = ClassNameResolver<ArchiveT>;

        static ClassNameResolver CreateSingleNameResolver(const std::string& name);
    private:
        CompositeScribe() = delete;
        CompositeScribe(const CompositeScribe& arg) = delete;
        CompositeScribe& operator=(const CompositeScribe& arg) = delete;
    private:
        static_assert(std::is_class_v<ObjectT>, "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object, class Archive>
    typename CompositeScribe<Object, Archive>::ClassNameResolver
        CompositeScribe<Object, Archive>::CreateSingleNameResolver(const std::string& name)
    {
        return ClassNameResolver([name](ArchiveT& archive)
        {
            return name;
        });
    }
}