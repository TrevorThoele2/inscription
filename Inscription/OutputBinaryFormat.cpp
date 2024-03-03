#include "OutputBinaryFormat.h"

namespace Inscription::Format
{
    OutputBinary::OutputBinary(Archive::OutputBinary& archive, const TypeRegistrationContext& typeRegistrationContext) :
		Binary(Direction::Output, typeRegistrationContext),
		archive(&archive)
    {}
    
    OutputBinary::OutputBinary(OutputBinary&& arg) noexcept :
        Binary(std::move(arg)), archive(arg.archive)
    {}

    OutputBinary& OutputBinary::operator=(OutputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        archive = arg.archive;
        return *this;
    }
}