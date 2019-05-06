
#include "OutputTextFile.h"
#include <iomanip>

namespace Inscription
{
    OutputTextFile::OutputTextFile(const Path& path, bool append) :
        SimpleFile(path, (!append) ? std::ios::out : std::ios::out | std::ios::app)
    {
        ResetFillCharacter();
        ResetWidth();
    }

    OutputTextFile& OutputTextFile::operator<<(const std::string& str)
    {
        if (width > 0)
        {
            stream << std::setfill(fillChar);
            stream << std::setw(width);
        }

        stream << str;
        return *this;
    }

    OutputTextFile& OutputTextFile::operator<<(const char ch)
    {
        stream << ch;
        return *this;
    }

    void OutputTextFile::ClearFile()
    {
        stream.flush();
    }

    void OutputTextFile::SetFillCharacter(const char set)
    {
        fillChar = set;
    }

    void OutputTextFile::ResetFillCharacter()
    {
        fillChar = ' ';
    }

    void OutputTextFile::SetWidth(Width set)
    {
        width = set;
    }

    void OutputTextFile::ResetWidth()
    {
        width = 0;
    }
}