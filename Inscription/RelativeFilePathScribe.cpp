#include "RelativeFilePathScribe.h"

#include "StringScribe.h"
#include "BinaryArchive.h"

#include "OutputBinaryArchive.h"

namespace Inscription
{
    void Scribe<::Chroma::RelativeFilePath, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto moveUpCount = object.GetMoveUpCount();
            archive(moveUpCount);

            auto moveDown = object.GetMoveDown();
            archive(moveDown);
        }
        else // INPUT
        {
            size_t moveUpCount;
            archive(moveUpCount);

            std::string moveDown;
            archive(moveDown);

            object = ::Chroma::RelativeFilePath(moveUpCount, moveDown);
        }
    }
}