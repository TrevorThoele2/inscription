#pragma once

#include "BinaryArchive.h"
#include "JsonArchive.h"
#include "TextArchive.h"

namespace Inscription
{
    template<class Object>
    class TrackingScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = true;
        using ScribeT = Scribe<Object>;
    public:
        static void Scriven(ObjectT& object, Archive::Binary& archive, ScribeT& scribe);
        static void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive, ScribeT& scribe);
        static void Scriven(ObjectT& object, Archive::Text& archive, ScribeT& scribe);
    };

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(ObjectT& object, Archive::Binary& archive, ScribeT& scribe)
    {
        {
            auto trackingID = archive.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                archive.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            scribe.Scriven(object, archive);
        }
    }

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive, ScribeT& scribe)
    {
        {
            auto trackingID = archive.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                archive.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(archive.types);
            scribe.Scriven(name, object, archive);
        }
    }

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(ObjectT& object, Archive::Text& archive, ScribeT& scribe)
    {
        scribe.Scriven(object, archive);
    }
}