#include "JsonArchive.h"
#include <utility>

#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    JsonArchive::~JsonArchive() = default;

    bool JsonArchive::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool JsonArchive::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputJsonArchive* JsonArchive::AsOutput()
    {
        return dynamic_cast<OutputJsonArchive*>(this);
    }

    InputJsonArchive* JsonArchive::AsInput()
    {
        return dynamic_cast<InputJsonArchive*>(this);
    }

    const OutputJsonArchive* JsonArchive::AsOutput() const
    {
        return dynamic_cast<const OutputJsonArchive*>(this);
    }

    const InputJsonArchive* JsonArchive::AsInput() const
    {
        return dynamic_cast<const InputJsonArchive*>(this);
    }

    JsonArchive::JsonArchive(Direction direction) :
        types(*this),
        direction(direction)
    {}

    JsonArchive::JsonArchive(Direction direction, TypeRegistrationContext typeRegistrationContext) :
        types(*this, typeRegistrationContext),
        direction(direction)
    {
        typeRegistrationContext.PushAll(*this);
    }

    JsonArchive::JsonArchive(JsonArchive&& arg) noexcept :
        Archive(std::move(arg)),
        types(std::move(arg.types), *this),
        direction(arg.direction)
    {}

    JsonArchive& JsonArchive::operator=(JsonArchive&& arg) noexcept
    {
        Archive::operator=(std::move(arg));
        types = std::move(arg.types);
        return *this;
    }
}