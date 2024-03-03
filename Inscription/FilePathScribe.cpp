#include "FilePathScribe.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

#include "StringScribe.h"

namespace Inscription
{
    void Scribe<std::filesystem::path>::Scriven(ObjectT& object, Archive::Binary& archive)
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

    void Scribe<std::filesystem::path>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        if (archive.IsOutput())
        {
            auto string = object.string();
            archive(name, string);
        }
        else
        {
            std::string string;
            archive(name, string);
            object = string;
        }
    }

    void Scribe<std::filesystem::path>::Scriven(ObjectT& object, Archive::Text& archive)
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