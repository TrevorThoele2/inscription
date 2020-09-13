#pragma once

#include "BinaryArchive.h"
#include "OutputBinaryFile.h"

#include "Endian.h"

namespace Inscription
{
    class OutputBinaryArchive final : public BinaryArchive
    {
    public:
        OutputBinaryArchive(const FilePath& path);
        OutputBinaryArchive(const FilePath& path, const TypeRegistrationContext& typeRegistrationContext);
        OutputBinaryArchive(OutputBinaryArchive&& arg) noexcept;

        OutputBinaryArchive& operator=(OutputBinaryArchive&& arg) noexcept;

        template<class T>
        OutputBinaryArchive& Write(T& object);
    public:
        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
    protected:
        void WriteImpl(bool arg) { WriteToFile(arg); }
        void WriteImpl(signed char arg) { WriteToFile(arg); }
        void WriteImpl(char arg) { WriteToFile(arg); }
        void WriteImpl(short arg) { WriteToFile(arg); }
        void WriteImpl(int arg) { WriteToFile(arg); }
        void WriteImpl(long arg) { WriteToFile(arg); }
        void WriteImpl(long long arg) { WriteToFile(arg); }
        void WriteImpl(unsigned char arg) { WriteToFile(arg); }
        void WriteImpl(unsigned short arg) { WriteToFile(arg); }
        void WriteImpl(unsigned int arg) { WriteToFile(arg); }
        void WriteImpl(unsigned long arg) { WriteToFile(arg); }
        void WriteImpl(unsigned long long arg) { WriteToFile(arg); }
        void WriteImpl(float arg) { WriteToFile(arg); }
        void WriteImpl(double arg) { WriteToFile(arg); }
        void WriteImpl(const Buffer &arg) { WriteToFile(arg); }
    private:
        OutputBinaryFile file;
    private:
        template<class T>
        void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };

    template<class T>
    OutputBinaryArchive& OutputBinaryArchive::Write(T& object)
    {
        static_assert(
            std::is_arithmetic_v<T> || std::is_same_v<T, Buffer>,
            "The T given to Write was not arithmetic or a Buffer.");

        if (!IsLittleEndian())
            EnsureCorrectEndianness(object);

        WriteImpl(object);

        return *this;
    }
}