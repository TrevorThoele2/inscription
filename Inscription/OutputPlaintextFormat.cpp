#include "OutputPlaintextFormat.h"

namespace Inscription::Format
{
    OutputPlaintext::OutputPlaintext(Archive::OutputText& archive) :
        Plaintext(Direction::Output), archive(&archive)
    {}
    
    OutputPlaintext::OutputPlaintext(OutputPlaintext&& arg) noexcept :
        Plaintext(std::move(arg)), archive(arg.archive)
    {}

    OutputPlaintext& OutputPlaintext::operator=(OutputPlaintext&& arg) noexcept
    {
        Plaintext::operator=(std::move(arg));
        archive = arg.archive;
        return *this;
    }

    OutputPlaintext& OutputPlaintext::Write(const std::string& arg)
    {
        archive->Write(arg);
        return *this;
    }

    OutputPlaintext& OutputPlaintext::Write(const char arg)
    {
        archive->Write(arg);
        return *this;
    }
}