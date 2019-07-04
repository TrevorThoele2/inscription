#include "InputTextArchive.h"

namespace Inscription
{
    InputTextArchive::InputTextArchive(const FilePath& path) :
        TextArchive(Direction::INPUT), file(path)
    {}

    InputTextArchive::InputTextArchive(InputTextArchive&& arg) :
        TextArchive(std::move(arg)), file(std::move(arg.file))
    {}

    InputTextArchive& InputTextArchive::operator=(InputTextArchive&& arg)
    {
        TextArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    InputTextArchive& InputTextArchive::ReadLine(std::string& arg)
    {
        ReadLineImpl(arg);
        return *this;
    }

    InputTextArchive& InputTextArchive::ReadLine(std::string& arg, char delimiter)
    {
        ReadLineImpl(arg, delimiter);
        return *this;
    }

    InputTextArchive& InputTextArchive::ReadSize(std::string& arg, size_t size)
    {
        ReadSizeImpl(arg, size);
        return *this;
    }

    void InputTextArchive::ReadLineFromFile(std::string& arg)
    {
        arg = file.ReadLine();
    }

    void InputTextArchive::ReadLineFromFile(std::string& arg, char delimiter)
    {
        arg = file.ReadLine(delimiter);
    }

    void InputTextArchive::ReadSizeFromFile(std::string& arg, size_t size)
    {
        arg = file.ReadSize(size);
    }
}