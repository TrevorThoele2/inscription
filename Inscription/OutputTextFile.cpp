#include "OutputTextFile.h"
#include <iomanip>

namespace Inscription
{
    OutputTextFile::OutputTextFile(const FilePath& path, bool append) :
        SimpleFile(path, (!append) ? std::ios::out : std::ios::out | std::ios::app)
    {
        ResetFillCharacter();
        ResetWidth();
    }

    OutputTextFile::OutputTextFile(OutputTextFile&& arg) : SimpleFile(std::move(arg))
    {}

    OutputTextFile& OutputTextFile::operator=(OutputTextFile&& arg)
    {
        SimpleFile::operator=(std::move(arg));
        return *this;
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