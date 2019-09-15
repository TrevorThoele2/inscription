#include "TextArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

namespace Inscription
{
    TextArchive::~TextArchive() = default;

    bool TextArchive::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool TextArchive::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputTextArchive* TextArchive::AsOutput()
    {
        return dynamic_cast<OutputTextArchive*>(this);
    }

    InputTextArchive* TextArchive::AsInput()
    {
        return dynamic_cast<InputTextArchive*>(this);
    }

    const OutputTextArchive* TextArchive::AsOutput() const
    {
        return dynamic_cast<const OutputTextArchive*>(this);
    }

    const InputTextArchive* TextArchive::AsInput() const
    {
        return dynamic_cast<const InputTextArchive*>(this);
    }

    TextArchive::TextArchive(Direction direction) : direction(direction)
    {}
    
    TextArchive::TextArchive(TextArchive&& arg) noexcept :
        Archive(std::move(arg)), direction(std::move(arg.direction))
    {}

    TextArchive& TextArchive::operator=(TextArchive&& arg) noexcept
    {
        Archive::operator=(std::move(arg));
        return *this;
    }
}