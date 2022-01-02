#pragma once

#include "BinaryFormat.h"
#include "JsonFormat.h"
#include "PlaintextFormat.h"

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
        static void Scriven(ObjectT& object, Format::Binary& format, ScribeT& scribe);
        static void Scriven(const std::string& name, ObjectT& object, Format::Json& format, ScribeT& scribe);
        static void Scriven(ObjectT& object, Format::Plaintext& format, ScribeT& scribe);
    };

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(ObjectT& object, Format::Binary& format, ScribeT& scribe)
    {
        {
            auto trackingID = format.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                format.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(format.types);
            scribe.Scriven(object, format);
        }
    }

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(const std::string& name, ObjectT& object, Format::Json& format, ScribeT& scribe)
    {
        {
            auto trackingID = format.types.AttemptTrackObject(&object);
            if (trackingID.has_value())
                format.types.TrackSavedConstruction(*trackingID);
        }

        {
            auto trackingContext = ObjectTrackingContext::Active(format.types);
            scribe.Scriven(name, object, format);
        }
    }

    template <class Object>
    void TrackingScribeCategory<Object>::Scriven(ObjectT& object, Format::Plaintext& format, ScribeT& scribe)
    {
        scribe.Scriven(object, format);
    }
}