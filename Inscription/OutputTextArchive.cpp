#include "OutputTextArchive.h"

namespace Inscription
{
    OutputTextArchive::OutputTextArchive(const FilePath& path, bool append) :
        TextArchive(Direction::Output), file(path, append)
    {}

    OutputTextArchive::OutputTextArchive(OutputTextArchive&& arg) noexcept :
        TextArchive(std::move(arg)), file(std::move(arg.file))
    {}

    OutputTextArchive& OutputTextArchive::operator=(OutputTextArchive&& arg) noexcept
    {
        TextArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    OutputTextArchive& OutputTextArchive::Write(const std::string& arg)
    {
        WriteImpl(arg);
        return *this;
    }

    OutputTextArchive& OutputTextArchive::Write(const char arg)
    {
        WriteImpl(arg);
        return *this;
    }
}