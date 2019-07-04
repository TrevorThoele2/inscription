#include "TextArchive.h"

#include "OutputTextArchive.h"
#include "InputTextArchive.h"

namespace Inscription
{
    TextArchive::~TextArchive()
    {}

    bool TextArchive::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool TextArchive::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    OutputTextArchive* TextArchive::AsOutput()
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<OutputTextArchive*>(this);
    }

    InputTextArchive* TextArchive::AsInput()
    {
        if (!IsInput())
            return nullptr;

        return static_cast<InputTextArchive*>(this);
    }

    const OutputTextArchive* TextArchive::AsOutput() const
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<const OutputTextArchive*>(this);
    }

    const InputTextArchive* TextArchive::AsInput() const
    {
        if (!IsInput())
            return nullptr;

        return static_cast<const InputTextArchive*>(this);
    }

    TextArchive::TextArchive(Direction direction) : direction(direction)
    {}
    
    TextArchive::TextArchive(TextArchive&& arg) : Archive(std::move(arg)), direction(std::move(arg.direction))
    {}

    TextArchive& TextArchive::operator=(TextArchive&& arg)
    {
        Archive::operator=(std::move(arg));
        return *this;
    }
}