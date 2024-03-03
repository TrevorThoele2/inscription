#include "FilePathScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

#include "StringScribe.h"

namespace Inscription
{
    void Scribe<std::filesystem::path, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto string = object.string();
            archive(string);
        }
        else
        {
            std::string string;
            archive(string);
            object = string;
        }
    }

    void Scribe<std::filesystem::path, TextArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto string = object.string();
            archive(string);
        }
        else
        {
            std::string string;
            archive(string);
            object = string;
        }
    }
}