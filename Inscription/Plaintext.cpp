#include "Plaintext.h"

#include "StringScribe.h"

namespace Inscription
{
    void Plaintext::ToFile(const std::string& string, const File::Path& path, bool append)
    {
        File::OutputText file(path, append);
        Archive::OutputText archive(file);
        Format::OutputPlaintext format(archive);
        format(string);
    }

    std::string Plaintext::FromFile(const File::Path& path)
    {
        File::InputText file(path);
        Archive::InputText archive(file);
        Format::InputPlaintext format(archive);
        std::string output;
        format(output);
        return output;
    }
}