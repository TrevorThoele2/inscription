#include "InputTextArchive.h"

namespace Inscription::Archive
{
    InputText::InputText(const File::Path& path) :
        Text(Direction::Input), file(path)
    {}

    InputText::InputText(InputText&& arg) noexcept :
        Text(std::move(arg)), file(std::move(arg.file))
    {}

    InputText& InputText::operator=(InputText&& arg) noexcept
    {
        Text::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    InputText& InputText::ReadLine(std::string& arg)
    {
        ReadLineImpl(arg);
        return *this;
    }

    InputText& InputText::ReadLine(std::string& arg, char delimiter)
    {
        ReadLineImpl(arg, delimiter);
        return *this;
    }

    InputText& InputText::ReadSize(std::string& arg, size_t size)
    {
        ReadSizeImpl(arg, size);
        return *this;
    }

    void InputText::ReadLineFromFile(std::string& arg)
    {
        arg = file.ReadLine();
    }

    void InputText::ReadLineFromFile(std::string& arg, char delimiter)
    {
        arg = file.ReadLine(delimiter);
    }

    void InputText::ReadSizeFromFile(std::string& arg, size_t size)
    {
        arg = file.ReadSize(size);
    }
}