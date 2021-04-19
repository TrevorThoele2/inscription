#include "OutputTextArchive.h"

namespace Inscription::Archive
{
    OutputText::OutputText(const File::Path& path, bool append) :
        Text(Direction::Output), file(path, append)
    {}

    OutputText::OutputText(OutputText&& arg) noexcept :
        Text(std::move(arg)), file(std::move(arg.file))
    {}

    OutputText& OutputText::operator=(OutputText&& arg) noexcept
    {
        Text::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    OutputText& OutputText::Write(const std::string& arg)
    {
        WriteImpl(arg);
        return *this;
    }

    OutputText& OutputText::Write(const char arg)
    {
        WriteImpl(arg);
        return *this;
    }
}