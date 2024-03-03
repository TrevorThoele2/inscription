#pragma once

#include <variant>

#include "BinaryArchive.h"
#include "InputBinaryFile.h"

#include "Endian.h"

namespace Inscription::Archive
{
    class InputBinary final : public Binary
    {
    public:
        InputBinary(File::InputBinary& file);
        InputBinary(Buffer& buffer);
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

        [[nodiscard]] File::InputBinary* File() const;
    private:
        using Source = std::variant<File::InputBinary*, Buffer*>;
        Source source;
    private:
        template<class T>
        void DoRead(T& arg);

        static void PullFromBuffer(Buffer& arg, Buffer& buffer);
        template<class T>
        static void PullFromBuffer(T arg, Buffer& buffer);
    };
    
    template<class T>
    void InputBinary::DoRead(T& arg)
    {
        static_assert(
            std::is_arithmetic_v<T> || std::is_same_v<T, Buffer>,
            "The T given to Read was not arithmetic or a Buffer.");

        if (std::holds_alternative<File::InputBinary*>(source))
            std::get<File::InputBinary*>(source)->ReadData(arg);
        else
            PullFromBuffer(arg, *std::get<Buffer*>(source));

        if (!IsLittleEndian())
            EnsureCorrectEndianness(arg);
    }

    template<class T>
	void InputBinary::PullFromBuffer(T arg, Buffer& buffer)
    {
        const auto bytes = reinterpret_cast<char*>(&arg);
        const auto size = sizeof(arg);
        for (size_t i = 0; i < size; ++i)
            bytes[i] = buffer.value[i];
        buffer.value.erase(buffer.value.begin(), buffer.value.begin() + size);
    }
}