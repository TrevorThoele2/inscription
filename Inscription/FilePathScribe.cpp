#include "FilePathScribe.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"

#include "StringScribe.h"

namespace Inscription
{
    void Scribe<std::filesystem::path>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            auto string = object.string();
            format(string);
        }
        else
        {
            std::string string;
            format(string);
            object = string;
        }
    }

    void Scribe<std::filesystem::path>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            auto string = object.string();
            format(name, string);
        }
        else
        {
            std::string string;
            format(name, string);
            object = string;
        }
    }

    void Scribe<std::filesystem::path>::Scriven(ObjectT& object, Format::Plaintext& format)
    {
        if (format.IsOutput())
        {
            auto string = object.string();
            format(string);
        }
        else
        {
            std::string string;
            format(string);
            object = string;
        }
    }
}