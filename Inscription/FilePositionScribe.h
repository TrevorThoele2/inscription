#pragma once

#include "FilePosition.h"
#include "TrackingScribeCategory.h"

namespace Inscription
{
    namespace Archive
    {
        class Binary;
    }

    template<>
    class Scribe<File::Position> final
    {
    public:
        using ObjectT = File::Position;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
    };

    template<class Archive>
    struct ScribeTraits<File::Position, Archive>
    {
        using Category = TrackingScribeCategory<File::Position>;
    };
}
