#pragma once

#include "StreamPosition.h"
#include "ObjectScribe.h"

namespace Inscription
{
    class BinaryArchive;

    template<>
    class Scribe<StreamPosition, BinaryArchive> final :
        public ObjectScribe<StreamPosition, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };
}
