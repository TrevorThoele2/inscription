#pragma once

#include "PlaintextFormat.h"
#include "InputTextArchive.h"

namespace Inscription::Format
{
    class InputPlaintext final : public Plaintext
    {
    public:
        explicit InputPlaintext(Archive::InputText& archive);
        InputPlaintext(const InputPlaintext& arg) = delete;
        InputPlaintext(InputPlaintext&& arg) noexcept;

        InputPlaintext& operator=(const InputPlaintext& arg) = delete;
        InputPlaintext& operator=(InputPlaintext&& arg) noexcept;

        InputPlaintext& ReadUntil(std::string& arg, char delimiter);
        InputPlaintext& ReadSize(std::string& arg, size_t size = std::numeric_limits<size_t>::max());
    private:
        Archive::InputText* archive;
    };
}