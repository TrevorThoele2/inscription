#pragma once

#include "BinaryFormat.h"
#include "InputBinaryArchive.h"

namespace Inscription::Format
{
    class InputBinary final : public Binary
    {
    public:
        InputBinary(Archive::InputBinary& archive, const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext());
        InputBinary(InputBinary&& arg) noexcept;

        InputBinary& operator=(InputBinary&& arg) noexcept;

        void Read(bool& arg) { DoRead(arg); }
        void Read(signed char& arg) { DoRead(arg); }
        void Read(char& arg) { DoRead(arg); }
        void Read(short& arg) { DoRead(arg); }
        void Read(int& arg) { DoRead(arg); }
        void Read(long& arg) { DoRead(arg); }
        void Read(long long& arg) { DoRead(arg); }
        void Read(unsigned char& arg) { DoRead(arg); }
        void Read(unsigned short& arg) { DoRead(arg); }
        void Read(unsigned int& arg) { DoRead(arg); }
        void Read(unsigned long& arg) { DoRead(arg); }
        void Read(unsigned long long& arg) { DoRead(arg); }
        void Read(float& arg) { DoRead(arg); }
        void Read(double& arg) { DoRead(arg); }
        void Read(Buffer& arg) { DoRead(arg); }

        [[nodiscard]] Archive::InputBinary& Archive() const;
    private:
        Archive::InputBinary* archive = nullptr;
    private:
        template<class T>
        void DoRead(T& arg);
    };

    template<class T>
    void InputBinary::DoRead(T& arg)
    {
        archive->Read(arg);
    }
}