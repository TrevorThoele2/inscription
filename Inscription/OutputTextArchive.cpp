#include "OutputTextArchive.h"

namespace Inscription::Archive
{
    OutputText::OutputText(File::OutputText& file) :
		Text(Direction::Output), source(&file)
    {}

    OutputText::OutputText(std::string& string) :
        Text(Direction::Output), source(&string)
    {}
    
    OutputText::OutputText(OutputText&& arg) noexcept :
        Text(std::move(arg)), source(std::move(arg.source))
    {}

    OutputText& OutputText::operator=(OutputText&& arg) noexcept
    {
        Text::operator=(std::move(arg));
        source = std::move(arg.source);
        return *this;
    }

    OutputText& OutputText::Write(const std::string& arg)
    {
        if (std::holds_alternative<File::OutputText*>(source))
            std::get<File::OutputText*>(source)->WriteData(arg);
        else
        {
            const auto stringSource = std::get<std::string*>(source);
            stringSource->insert(stringSource->end(), arg.begin(), arg.end());
        }

        return *this;
    }

    OutputText& OutputText::Write(const char arg)
    {
        if (std::holds_alternative<File::OutputText*>(source))
            std::get<File::OutputText*>(source)->WriteData(arg);
        else
        {
            const auto stringSource = std::get<std::string*>(source);
            stringSource->push_back(arg);
        }

        return *this;
    }
}