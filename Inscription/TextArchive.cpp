#include "TextArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

namespace Inscription::Archive
{
    Text::~Text() = default;

    bool Text::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool Text::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputText* Text::AsOutput()
    {
        return dynamic_cast<OutputText*>(this);
    }

    InputText* Text::AsInput()
    {
        return dynamic_cast<InputText*>(this);
    }

    const OutputText* Text::AsOutput() const
    {
        return dynamic_cast<const OutputText*>(this);
    }

    const InputText* Text::AsInput() const
    {
        return dynamic_cast<const InputText*>(this);
    }

    Text::Text(Direction direction) : direction(direction)
    {}
    
    Text::Text(Text&& arg) noexcept :
        Archive(std::move(arg)), direction(std::move(arg.direction))
    {}

    Text& Text::operator=(Text&& arg) noexcept
    {
        Archive::operator=(std::move(arg));
        return *this;
    }
}