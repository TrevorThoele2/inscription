#pragma once

#include "Buffer.h"

#include "BinaryArchive.h"

namespace Inscription
{
    template<>
    class Scribe<Buffer> final
    {
    public:
        using ObjectT = Buffer;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
    };

    template<class Archive>
    struct ScribeTraits<Buffer, Archive>
    {
        static constexpr bool shouldTrack = false;
    };
}