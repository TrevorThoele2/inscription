#pragma once

#include "ScribeBase.h"
#include "BinaryArchive.h"
#include "JsonArchive.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TrackingScribe;

    template<class Object>
    class TrackingScribe<Object, BinaryArchive> : public ScribeBase<Object, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override;
    protected:
        TrackingScribe() = default;
        TrackingScribe(const TrackingScribe& arg) = default;

        virtual void ScrivenImplementation(ObjectT& object, ArchiveT& archive) = 0;
    };

    template<class Object>
    void TrackingScribe<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        {
            auto trackingID = archive.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                archive.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            ScrivenImplementation(object, archive);
        }
    }

    template<class Object>
    class TrackingScribe<Object, JsonArchive> : public ScribeBase<Object, JsonArchive>
    {
    private:
        using BaseT = ScribeBase<Object, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
        void Scriven(const std::string& name, ObjectT& object, ArchiveT& archive) override;
    protected:
        TrackingScribe() = default;
        TrackingScribe(const TrackingScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    };

    template<class Object>
    void TrackingScribe<Object, JsonArchive>::Scriven(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        {
            auto trackingID = archive.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                archive.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            ScrivenImplementation(name, object, archive);
        }
    }
}