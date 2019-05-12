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

    void OutputTextFile::WriteData(const std::string& string)
    {
        if (width > 0)
        {
            stream << std::setfill(fillCharacter);
            stream << std::setw(width);
        }

        stream << string;
    }

    void OutputTextFile::WriteData(const char character)
    {
        stream << character;
    }

    void OutputTextFile::ClearFile()
    {
        stream.flush();
    }

    void OutputTextFile::SetFillCharacter(const char set)
    {
        fillCharacter = set;
    }

    void OutputTextFile::ResetFillCharacter()
    {
        fillCharacter = ' ';
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