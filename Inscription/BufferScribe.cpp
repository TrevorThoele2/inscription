#include "BufferScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    void Scribe<Buffer, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->Write(object);
        else
            archive.AsInput()->Read(object);
    }

    void Scribe<Buffer, BinaryArchive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}