#pragma once

#include "BinaryArchive.h"
#include "InputBinaryFile.h"

#include "Endian.h"

namespace Inscription::Archive
{
    class InputBinary final : public Binary
    {
    public:
        InputBinary(const File::Path& path);
        InputBinary(const File::Path& path, const TypeRegistrationContext& typeRegistrationContext);
        InputBinary(InputBinary&& arg) noexcept;

        InputBinary& operator=(InputBinary&& arg) noexcept;

        template<class T>
        InputBinary& Read(T& object);
    public:
        void Seek(File::Position offset) override;
        void SeekFromBeginning(File::Position offset = 0) override;
        void SeekFromEnd(File::Position offset = 0) override;
        [[nodiscard]] File::Position Position() override;
        [[nodiscard]] File::Size Size();
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
        void ReadImpl(Buffer& arg) { file.ReadData(arg.value, arg.value.size()); }
    private:
        File::InputBinary file;
    private:
        template<class T>
        void ReadFromFile(T& arg)
        {
            file.ReadData(arg);
        }
    };

    template<class T>
    InputBinary& InputBinary::Read(T& object)
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