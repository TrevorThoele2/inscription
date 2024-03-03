#pragma once

#include "BinaryArchive.h"
#include "InputBinaryFile.h"

#include "Endian.h"

namespace Inscription
{
    class InputBinaryArchive final : public BinaryArchive
    {
    public:
        using SizeT = std::streamsize;
    public:
        InputBinaryArchive(const FilePath& path);
        InputBinaryArchive(const FilePath& path, const TypeRegistrationContext& typeRegistrationContext);
        InputBinaryArchive(InputBinaryArchive&& arg) noexcept;

        InputBinaryArchive& operator=(InputBinaryArchive&& arg) noexcept;

        template<class T>
        InputBinaryArchive& Read(T& object);
    public:
        void SeekStreamFromCurrent(StreamPosition offset) override;
        void SeekStreamFromBegin(StreamPosition offset = 0) override;
        void SeekStreamFromEnd(StreamPosition offset = 0) override;
        [[nodiscard]] StreamPosition CurrentStreamPosition() override;
        [[nodiscard]] SizeT Size();
    protected:
        void ReadImpl(bool& arg) { ReadFromFile(arg); }
        void ReadImpl(signed char& arg) { ReadFromFile(arg); }
        void ReadImpl(char& arg) { ReadFromFile(arg); }
        void ReadImpl(short& arg) { ReadFromFile(arg); }
        void ReadImpl(int& arg) { ReadFromFile(arg); }
        void ReadImpl(long& arg) { ReadFromFile(arg); }
        void ReadImpl(long long& arg) { ReadFromFile(arg); }
        void ReadImpl(unsigned char& arg) { ReadFromFile(arg); }
        void ReadImpl(unsigned short& arg) { ReadFromFile(arg); }
        void ReadImpl(unsigned int& arg) { ReadFromFile(arg); }
        void ReadImpl(unsigned long& arg) { ReadFromFile(arg); }
        void ReadImpl(unsigned long long&  arg) { ReadFromFile(arg); }
        void ReadImpl(float& arg) { ReadFromFile(arg); }
        void ReadImpl(double& arg) { ReadFromFile(arg); }
        void ReadImpl(Buffer& arg) { file.ReadData(arg, arg.size()); }
    private:
        InputBinaryFile file;
    private:
        template<class T>
        void ReadFromFile(T& arg)
        {
            file.ReadData(arg);
        }
    };

    template<class T>
    InputBinaryArchive& InputBinaryArchive::Read(T& object)
    {
        static_assert(
            std::is_arithmetic_v<T> || std::is_same_v<T, Buffer>,
            "The T given to Read was not arithmetic or a Buffer.");

        ReadImpl(object);

        if (!IsLittleEndian())
            EnsureCorrectEndianness(object);

        return *this;
    }
}