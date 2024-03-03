#pragma once

#include "PlaintextFormat.h"
#include "OutputTextArchive.h"

namespace Inscription::Format
{
    class OutputPlaintext final : public Plaintext
    {
    public:
        explicit OutputPlaintext(Archive::OutputText& archive);
        OutputPlaintext(const OutputPlaintext& arg) = delete;
        OutputPlaintext(OutputPlaintext&& arg) noexcept;

        OutputPlaintext& operator=(const OutputPlaintext& arg) = delete;
        OutputPlaintext& operator=(OutputPlaintext&& arg) noexcept;

        OutputPlaintext& Write(const std::string& arg);
        OutputPlaintext& Write(const char arg);
    private:
        Archive::OutputText* archive;
    };
}