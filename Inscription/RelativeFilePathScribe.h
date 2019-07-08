#pragma once

#include <Chroma/RelativeFilePath.h>
#include "CompositeScribe.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<::Chroma::RelativeFilePath, BinaryArchive> final :
        public CompositeScribe<::Chroma::RelativeFilePath, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}