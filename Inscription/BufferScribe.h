#pragma once

#include "ScribeBase.h"
#include "Buffer.h"

#include "BinaryArchive.h"

namespace Inscription
{
    template<>
    class Scribe<Buffer, BinaryArchive> : public ScribeBase<Buffer, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
    };

    template<>
    struct ObjectTrackingTraits<Buffer, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };
}