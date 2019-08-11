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
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };
}