#pragma once

#include <variant>
#include "BinaryArchive.h"
#include "OutputBinaryFile.h"

#include "Endian.h"

namespace Inscription::Archive
{
    class OutputBinary final : public Binary
    {
    public:
        OutputBinary(File::OutputBinary& file);
        OutputBinary(Buffer& buffer);
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

        [[nodiscard]] File::OutputBinary* File() const;
    private:
        using Sink = std::variant<File::OutputBinary*, Buffer*>;
        Sink sink;
    private:
        template<class T>
        void DoWrite(T& arg);

        static void PushToBuffer(const Buffer& arg, Buffer& buffer);
        template<class T>
        static void PushToBuffer(T arg, Buffer& buffer);
    };
    
    template<class T>
    void OutputBinary::DoWrite(T& arg)
    {
        static_assert(
            std::is_arithmetic_v<T> || std::is_same_v<T, Buffer>,
            "The T given to Write was not arithmetic or a Buffer.");

        if (!IsLittleEndian())
            EnsureCorrectEndianness(arg);

        if (std::holds_alternative<File::OutputBinary*>(sink))
            std::get<File::OutputBinary*>(sink)->WriteData(arg);
        else
            PushToBuffer(arg, *std::get<Buffer*>(sink));
    }

    template<class T>
    void OutputBinary::PushToBuffer(T arg, Buffer& buffer)
    {
        const auto bytes = reinterpret_cast<const char*>(&arg);
        for (size_t i = 0; i < sizeof(arg); ++i)
            buffer.value.push_back(bytes[i]);
    }
}