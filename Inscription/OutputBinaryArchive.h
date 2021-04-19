#pragma once

#include "BinaryArchive.h"
#include "OutputBinaryFile.h"

#include "Endian.h"

namespace Inscription::Archive
{
    class OutputBinary final : public Binary
    {
    public:
        OutputBinary(const File::Path& path);
        OutputBinary(const File::Path& path, const TypeRegistrationContext& typeRegistrationContext);
        OutputBinary(OutputBinary&& arg) noexcept;

        OutputBinary& operator=(OutputBinary&& arg) noexcept;

        template<class T>
        OutputBinary& Write(T& object);
    public:
        void Seek(File::Position position) override;
        void SeekFromBeginning(File::Position offset = 0) override;
        void SeekFromEnd(File::Position offset = 0) override;
        [[nodiscard]] File::Position Position() override;
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
        void WriteImpl(const Buffer& arg) { WriteToFile(arg); }
    private:
        File::OutputBinary file;
    private:
        template<class T>
        void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };

    template<class T>
    OutputBinary& OutputBinary::Write(T& object)
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