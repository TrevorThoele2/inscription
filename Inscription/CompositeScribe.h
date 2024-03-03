#pragma once

#include "Scribe.h"
#include "ScribeBase.h"

#include "BaseScriven.h"
#include "ClassNameResolver.h"

#include "Optional.h"
#include "TrackingID.h"
#include "ObjectTrackingContext.h"

#include "Assert.h"

#include "BinaryArchive.h"

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
    public:
        using ClassNameResolver = ClassNameResolver<ArchiveT>;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override;
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object, class Archive>
    void CompositeScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }

    template<class Object>
    class CompositeScribe<Object, BinaryArchive> : public ScribeBase<Object, BinaryArchive>
    {
    private:
        using BaseT = typename ScribeBase<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using ClassNameResolver = ClassNameResolver<ArchiveT>;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override;
        void Construct(ObjectT* storage, ArchiveT& archive) override;
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        using BaseT::ScrivenImplementation;
        using BaseT::ConstructImplementation;
        using BaseT::DoBasicConstruction;
    protected:
        static ClassNameResolver CreateSingleNameResolver(const std::string& name);
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribe was not a composite.");
    };

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        {
            auto trackingID = archive.AttemptTrackObject(&object);
            if (trackingID.IsValid())
                archive.TrackSavedConstruction(*trackingID);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            ScrivenImplementation(object, archive);
        }
    }

    template<class Object>
    void CompositeScribe<Object, BinaryArchive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        {
            archive.AttemptTrackObject(storage);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            ConstructImplementation(storage, archive);
        }
    }

    template<class Object>
    auto CompositeScribe<Object, BinaryArchive>::CreateSingleNameResolver(const std::string& name)
        -> CompositeScribe<Object, BinaryArchive>::ClassNameResolver
    {
        return ClassNameResolver([name](ArchiveT& archive)
        {
            return name;
        });
    }
}

#include "UndefAssert.h"