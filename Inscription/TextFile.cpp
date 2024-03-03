
#include "TextFile.h"
#include <iomanip>

namespace inscription
{
    TextInFile::TextInFile(const std::string &name) : SimpleFile(name, std::ios::in)
    {}

    void TextInFile::GetLine(std::string &str)
    {
        std::getline(stream, str);
    }

    void TextInFile::GetLine(std::string &str, char delimiter)
    {
        std::getline(stream, str, delimiter);
    }

    TextOutFile::TextOutFile(const std::string &name, bool append) : SimpleFile(name, (!append) ? std::ios::out : std::ios::out | std::ios::app)
    {
        ResetFillCharacter();
        ResetWidth();
    }

    TextOutFile& TextOutFile::operator<<(const std::string &str)
    {
        if (width > 0)
        {
            stream << std::setfill(fillChar);
            stream << std::setw(width);
        }

        stream << str;
        return *this;
    }

    TextOutFile& TextOutFile::operator<<(const char ch)
    {
        stream << ch;
        return *this;
    }

    void TextOutFile::Flush()
    {
        stream.flush();
    }

    void TextOutFile::SetFillCharacter(const char set)
    {
        fillChar = set;
    }

    void TextOutFile::ResetFillCharacter()
    {
        fillChar = ' ';
    }

    void TextOutFile::SetWidth(std::streamsize set)
    {
        width = set;
    }

    void TextOutFile::ResetWidth()
    {
        width = 0;
    }
}