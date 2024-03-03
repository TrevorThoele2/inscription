#pragma once

#include "BinaryFormat.h"
#include "OutputBinaryArchive.h"

namespace Inscription::Format
{
    class OutputBinary final : public Binary
    {
    public:
        OutputBinary(Archive::OutputBinary& archive, const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext());
        OutputBinary(OutputBinary&& arg) noexcept;

        OutputBinary& operator=(OutputBinary&& arg) noexcept;

        void Write(bool arg) { DoWrite(arg); }
        void Write(signed char arg) { DoWrite(arg); }
        void Write(char arg) { DoWrite(arg); }
        void Write(short arg) { DoWrite(arg); }
        void Write(int arg) { DoWrite(arg); }
        void Write(long arg) { DoWrite(arg); }
        void Write(long long arg) { DoWrite(arg); }
        void Write(unsigned char arg) { DoWrite(arg); }
        void Write(unsigned short arg) { DoWrite(arg); }
        void Write(unsigned int arg) { DoWrite(arg); }
        void Write(unsigned long arg) { DoWrite(arg); }
        void Write(unsigned long long arg) { DoWrite(arg); }
        void Write(float arg) { DoWrite(arg); }
        void Write(double arg) { DoWrite(arg); }
        void Write(Buffer& arg) { DoWrite(arg); }

        [[nodiscard]] Archive::OutputBinary& Archive() const;
    private:
        Archive::OutputBinary* archive;
    private:
        template<class T>
        void DoWrite(T& arg);
    };

    template<class T>
    void OutputBinary::DoWrite(T& arg)
    {
        archive->Write(arg);
    }
}