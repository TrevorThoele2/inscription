#include "FileNameScribe.h"

#include "StringScribe.h"
#include "BinaryArchive.h"

namespace Inscription
{
    void Scribe<::Chroma::FileName, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            archive(object.GetValue());
        else
        {
            std::string value;
            archive(value);

            object.Set(value);
        }
    }
}