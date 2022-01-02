#include "InputTextArchive.h"

namespace Inscription::Archive
{
    InputText::InputText(File::InputText& file) : Text(Direction::Input), source(&file)
    {}

	InputText::InputText(std::string& string) : Text(Direction::Input), source(StringSource(0, string))
    {}
    
    InputText::InputText(InputText&& arg) noexcept :
        Text(std::move(arg)), source(std::move(arg.source))
    {}

    InputText& InputText::operator=(InputText&& arg) noexcept
    {
        Text::operator=(std::move(arg));
        source = std::move(arg.source);
        return *this;
    }

    InputText& InputText::ReadUntil(std::string& arg, char delimiter)
    {
        if (std::holds_alternative<File::InputText*>(source))
            arg = std::get<File::InputText*>(source)->ReadUntil(delimiter);
        else
        {
            arg = {};
            auto& source = std::get<StringSource>(this->source);
            size_t i = source.position;
            while (i < source.string.size() && source.string[i] != delimiter)
            {
                arg.push_back(source.string[i]);
                ++i;
            }
            source.position += arg.size();
        }

        return *this;
    }

    InputText& InputText::ReadSize(std::string& arg, size_t size)
    {
        if (std::holds_alternative<File::InputText*>(source))
            arg = std::get<File::InputText*>(source)->ReadSize(size);
        else
        {
            arg = {};
            auto& source = std::get<StringSource>(this->source);
            for (size_t i = source.position; i < source.string.size() && arg.size() < size; ++i)
                arg.push_back(source.string[i]);
            source.position += arg.size();
        }

        return *this;
    }
}