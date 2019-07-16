#pragma once

#include <Chroma/FileName.h>
#include "CompositeScribe.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<::Chroma::FileName, BinaryArchive> final :
        public CompositeScribe<::Chroma::FileName, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };
}