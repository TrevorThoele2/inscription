#include "OutputTextFile.h"
#include <iomanip>

namespace Inscription::File
{
    OutputText::OutputText(const Path& path, bool append) :
        path(path),
        stream(path, append ? std::ios::out | std::ios::app : std::ios::out)
    {}

    OutputText::OutputText(OutputText&& arg) noexcept :
        path(std::move(arg.path)),
        stream(std::move(arg.stream))
    {}

    OutputText& OutputText::operator=(OutputText&& arg) noexcept
    {
        path = std::move(arg.path);
        stream = std::move(arg.stream);
        return *this;
    }

    void OutputText::WriteData(const std::string& string)
    {
        SanitizeStreamFailure(
            [this, string]()
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
            },
            path);
    }

    void OutputText::WriteData(const char character)
    {
        SanitizeStreamFailure([this, character]() { stream << character; }, path);
    }

    void OutputText::ClearFile()
    {
        SanitizeStreamFailure([this]() {stream.flush(); }, path);
    }

    void OutputText::SetFillCharacter(const char set)
    {
        fillCharacter = set;
    }

    void OutputText::ResetFillCharacter()
    {
        fillCharacter = ' ';
    }

    void OutputText::SetWidth(Width set)
    {
        width = set;
    }

    void OutputText::ResetWidth()
    {
        width = 0;
    }

    void OutputText::Seek(File::Position position)
    {
        SanitizeStreamFailure([this, position]() { stream.seekp(position); }, path);
    }

    Position OutputText::Position()
    {
        return SanitizeStreamFailure<File::Position>([this]() { return stream.tellp(); }, path);
    }
}