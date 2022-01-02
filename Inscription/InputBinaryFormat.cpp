#include "InputBinaryFormat.h"

namespace Inscription::Format
{
    InputBinary::InputBinary(Archive::InputBinary& archive, const TypeRegistrationContext& typeRegistrationContext) :
        Binary(Direction::Input, typeRegistrationContext),
		archive(&archive)
    {}

    InputBinary::InputBinary(InputBinary&& arg) noexcept :
        Binary(std::move(arg)), archive(arg.archive)
    {}

    InputBinary& InputBinary::operator=(InputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        archive = arg.archive;
        return *this;
    }
}