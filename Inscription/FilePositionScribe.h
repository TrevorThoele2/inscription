#pragma once

#include "FilePosition.h"
#include "TrackingScribeCategory.h"

namespace Inscription
{
    namespace Format
    {
        class Binary;
    }

    template<>
    class Scribe<File::Position> final
    {
    public:
        using ObjectT = File::Position;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<File::Position, Format>
    {
        using Category = TrackingScribeCategory<File::Position>;
    };
}
