#include "BinaryArchive.h"
#include <utility>

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    BinaryArchive::~BinaryArchive() = default;

    bool BinaryArchive::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool BinaryArchive::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputBinaryArchive* BinaryArchive::AsOutput()
    {
        return dynamic_cast<OutputBinaryArchive*>(this);
    }

    InputBinaryArchive* BinaryArchive::AsInput()
    {
        return dynamic_cast<InputBinaryArchive*>(this);
    }

    const OutputBinaryArchive* BinaryArchive::AsOutput() const
    {
        return dynamic_cast<const OutputBinaryArchive*>(this);
    }

    const InputBinaryArchive* BinaryArchive::AsInput() const
    {
        return dynamic_cast<const InputBinaryArchive*>(this);
    }

    BinaryArchive::BinaryArchive(Direction direction) :
        types(*this),
        direction(direction)
    {}

    BinaryArchive::BinaryArchive(Direction direction, TypeRegistrationContext typeRegistrationContext) :
        types(*this, typeRegistrationContext),
        direction(direction)
    {
        typeRegistrationContext.PushAll(*this);
    }

    BinaryArchive::BinaryArchive(BinaryArchive&& arg) noexcept :
        Archive(std::move(arg)),
        types(std::move(arg.types), *this),
        direction(arg.direction)
    {}

    BinaryArchive& BinaryArchive::operator=(BinaryArchive&& arg) noexcept
    {
        Archive::operator=(std::move(arg));
        types = std::move(arg.types);
        return *this;
    }
}