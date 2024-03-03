#include "BinaryArchive.h"
#include <utility>

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription::Archive
{
    Binary::~Binary() = default;

    bool Binary::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool Binary::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputBinary* Binary::AsOutput()
    {
        return dynamic_cast<OutputBinary*>(this);
    }

    InputBinary* Binary::AsInput()
    {
        return dynamic_cast<InputBinary*>(this);
    }

    const OutputBinary* Binary::AsOutput() const
    {
        return dynamic_cast<const OutputBinary*>(this);
    }

    const InputBinary* Binary::AsInput() const
    {
        return dynamic_cast<const InputBinary*>(this);
    }

    Binary::Binary(Direction direction) :
        types(*this),
        direction(direction)
    {}

    Binary::Binary(Direction direction, TypeRegistrationContext typeRegistrationContext) :
        types(*this, typeRegistrationContext),
        direction(direction)
    {
        typeRegistrationContext.PushAll(*this);
    }

    Binary::Binary(Binary&& arg) noexcept :
        Archive(std::move(arg)),
        types(std::move(arg.types), *this),
        direction(arg.direction)
    {}

    Binary& Binary::operator=(Binary&& arg) noexcept
    {
        Archive::operator=(std::move(arg));
        types = std::move(arg.types);
        return *this;
    }
}