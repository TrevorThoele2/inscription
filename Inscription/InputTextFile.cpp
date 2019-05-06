
#include "InputTextFile.h"

namespace Inscription
{
    InputTextFile::InputTextFile(const Path& path) : SimpleFile(path, std::ios::in)
    {}

    InputTextFile& InputTextFile::operator>>(std::string& str)
    {
        stream >> str;
        return *this;
    }

    InputTextFile& InputTextFile::operator>>(char& ch)
    {
        stream >> ch;
        return *this;
    }

    std::string InputTextFile::GetLine()
    {
        std::string str;
        std::getline(stream, str);
        return str;
    }

    std::string InputTextFile::GetLine(char delimiter)
    {
        std::string str;
        std::getline(stream, str, delimiter);
        return str;
    }
}