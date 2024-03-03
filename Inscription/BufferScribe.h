#pragma once

#include "Buffer.h"

#include "BinaryFormat.h"

namespace Inscription
{
    template<>
    class Scribe<Buffer> final
    {
    public:
        using ObjectT = Buffer;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
    };

    template<class Format>
    struct ScribeTraits<Buffer, Format>
    {
        static constexpr bool shouldTrack = false;
    };
}