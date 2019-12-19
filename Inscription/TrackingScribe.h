#pragma once

#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class TrackingScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override;
    protected:
        TrackingScribe() = default;
        TrackingScribe(const TrackingScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    };

    template<class Object, class Archive>
    void TrackingScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        {
            auto trackingID = archive.AttemptTrackObject(&object);
            if (trackingID.has_value())
                archive.TrackSavedConstruction(*trackingID);
        }

        {
            ObjectTrackingContext trackingContext(ObjectTrackingContext::Active, archive);
            ScrivenImplementation(object, archive);
        }
    }
}