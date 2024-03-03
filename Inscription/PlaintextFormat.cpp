#include "PlaintextFormat.h"
#include <utility>

#include "OutputPlaintextFormat.h"
#include "InputPlaintextFormat.h"

namespace Inscription::Format
{
    Plaintext::~Plaintext() = default;

    bool Plaintext::IsOutput() const
    {
        return direction == Direction::Output;
    }

    bool Plaintext::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputPlaintext* Plaintext::AsOutput()
    {
        return dynamic_cast<OutputPlaintext*>(this);
    }

    InputPlaintext* Plaintext::AsInput()
    {
        return dynamic_cast<InputPlaintext*>(this);
    }

    const OutputPlaintext* Plaintext::AsOutput() const
    {
        return dynamic_cast<const OutputPlaintext*>(this);
    }

    const InputPlaintext* Plaintext::AsInput() const
    {
        return dynamic_cast<const InputPlaintext*>(this);
    }

    Plaintext::Plaintext(Direction direction) :
        types(*this),
        direction(direction)
    {}

    Plaintext::Plaintext(Direction direction, TypeRegistrationContext typeRegistrationContext) :
        types(*this, typeRegistrationContext),
        direction(direction)
    {
        typeRegistrationContext.PushAll(*this);
    }

    Plaintext::Plaintext(Plaintext&& arg) noexcept :
        Format(std::move(arg)),
        types(std::move(arg.types), *this),
        direction(arg.direction)
    {}

    Plaintext& Plaintext::operator=(Plaintext&& arg) noexcept
    {
        Format::operator=(std::move(arg));
        types = std::move(arg.types);
        return *this;
    }
}