#include "OutputTextFile.h"
#include <iomanip>

namespace Inscription
{
    OutputTextFile::OutputTextFile(const FilePath& path, bool append) :
        SimpleFile(path, append ? std::ios::out | std::ios::app : std::ios::out)
    {}

    OutputTextFile::OutputTextFile(OutputTextFile&& arg) noexcept :
        SimpleFile(std::move(arg))
    {}

    OutputTextFile& OutputTextFile::operator=(OutputTextFile&& arg) noexcept
    {
        SimpleFile::operator=(std::move(arg));
        return *this;
    }

    void OutputTextFile::WriteData(const std::string& string)
    {
        if (width > 0)
        {
            const auto previousFill = stream.fill();
            const auto previousWidth = stream.width();

            stream.fill(fillCharacter);
            stream.width(width);

            stream << string;

            stream.fill(previousFill);
            stream.width(previousWidth);
        }
        else
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

    void OutputTextFile::SeekStream(StreamPosition position)
    {
        stream.seekp(position);
    }

    StreamPosition OutputTextFile::TellStream()
    {
        return stream.tellp();
    }
}