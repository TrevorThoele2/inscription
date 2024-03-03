#pragma once

#include "ScribeBase.h"
#include "Buffer.h"

#include "BinaryArchive.h"

namespace Inscription
{
    template<>
    class Scribe<Buffer, BinaryArchive> : public ScribeBase<Buffer, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}