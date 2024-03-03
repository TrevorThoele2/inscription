#pragma once

#include <Chroma/FilePath.h>
#include "CompositeScribe.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<::Chroma::FilePath, BinaryArchive> final :
        public CompositeScribe<::Chroma::FilePath, BinaryArchive>
    {
    public:
        static void ScrivenImplementation(ObjectT& object, ArchiveT& archive);
    };
}