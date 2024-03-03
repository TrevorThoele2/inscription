#pragma once

#include "StreamPosition.h"
#include "TrackingScribeCategory.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<StreamPosition> final
    {
    public:
        using ObjectT = StreamPosition;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<StreamPosition, Archive>
    {
        using Category = TrackingScribeCategory<StreamPosition>;
    };
}
