#include "InputPlaintextFormat.h"

namespace Inscription::Format
{
    InputPlaintext::InputPlaintext(Archive::InputText& archive) :
        Plaintext(Direction::Input), archive(&archive)
    {}
    
    InputPlaintext::InputPlaintext(InputPlaintext&& arg) noexcept :
        Plaintext(std::move(arg)), archive(arg.archive)
    {}

    InputPlaintext& InputPlaintext::operator=(InputPlaintext&& arg) noexcept
    {
        Format::operator=(std::move(arg));
        archive = arg.archive;
        return *this;
    }

    InputPlaintext& InputPlaintext::ReadUntil(std::string& arg, char delimiter)
    {
        archive->ReadUntil(arg, delimiter);
        return *this;
    }

    InputPlaintext& InputPlaintext::ReadSize(std::string& arg, size_t size)
    {
        archive->ReadSize(arg, size);
        return *this;
    }
}