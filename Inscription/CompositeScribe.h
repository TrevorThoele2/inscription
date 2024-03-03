#pragma once

#include "Scribe.h"
#include "ScribeBase.h"

#include "BaseScriven.h"
#include "ClassNameResolver.h"

namespace Inscription
{
    class BinaryArchive;

    template<class Object, class Archive>
    class CompositeScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = typename ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using ClassNameResolver = ClassNameResolver<ArchiveT>;
    public:
        template<class T>
        using ComposingScribe = Scribe<T, ArchiveT>;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    protected:
        static ClassNameResolver CreateSingleNameResolver(const std::string& name);
    private:
        using DerivedScribe = Scribe<ObjectT, ArchiveT>;
    private:
        static void DoTrackObject(ObjectT& object, ArchiveT& archive, std::true_type);
        static void DoTrackObject(ObjectT& object, ArchiveT& archive, std::false_type);
    private:
        CompositeScribe() = delete;
        CompositeScribe(const CompositeScribe& arg) = delete;
        CompositeScribe& operator=(const CompositeScribe& arg) = delete;
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object, class Archive>
    void CompositeScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        DoTrackObject(object, archive, std::bool_constant<std::is_same_v<ArchiveT, BinaryArchive>>{});
        DerivedScribe::ScrivenImplementation(object, archive);
    }

    template<class Object, class Archive>
    typename CompositeScribe<Object, Archive>::ClassNameResolver
        CompositeScribe<Object, Archive>::CreateSingleNameResolver(const std::string& name)
    {
        return ClassNameResolver([name](ArchiveT& archive)
        {
            return name;
        });
    }

    template<class Object, class Archive>
    void CompositeScribe<Object, Archive>::DoTrackObject(ObjectT& object, ArchiveT& archive, std::true_type)
    {
        archive.TrackObject(&object);
    }

    template<class Object, class Archive>
    void CompositeScribe<Object, Archive>::DoTrackObject(ObjectT& object, ArchiveT& archive, std::false_type)
    {}
}