#include "RelativeFilePathScribe.h"

#include "StringScribe.h"
#include "BinaryArchive.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    void Scribe<::Chroma::RelativeFilePath, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto moveUpCount = object.GetMoveUpCount();
            archive(moveUpCount);

            auto moveDown = object.GetMoveDown();
            archive(moveDown);
        }
        else // Input
        {
            size_t moveUpCount;
            archive(moveUpCount);

            std::string moveDown;
            archive(moveDown);

            object = ::Chroma::RelativeFilePath(moveUpCount, moveDown);
        }
    }
}