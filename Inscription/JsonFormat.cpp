#include "JsonFormat.h"
#include <utility>

#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription::Format
{
    Json::~Json() = default;

    bool Json::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool Json::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputJson* Json::AsOutput()
    {
        return dynamic_cast<OutputJson*>(this);
    }

    InputJson* Json::AsInput()
    {
        return dynamic_cast<InputJson*>(this);
    }

    const OutputJson* Json::AsOutput() const
    {
        return dynamic_cast<const OutputJson*>(this);
    }

    const InputJson* Json::AsInput() const
    {
        return dynamic_cast<const InputJson*>(this);
    }

    Json::Json(Direction direction) :
        types(*this),
        direction(direction)
    {}

    Json::Json(Direction direction, TypeRegistrationContext typeRegistrationContext) :
        types(*this, typeRegistrationContext),
        direction(direction)
    {
        typeRegistrationContext.PushAll(*this);
    }

    Json::Json(Json&& arg) noexcept :
        Format(std::move(arg)),
        types(std::move(arg.types), *this),
        direction(arg.direction)
    {}

    Json& Json::operator=(Json&& arg) noexcept
    {
        Format::operator=(std::move(arg));
        types = std::move(arg.types);
        return *this;
    }
}