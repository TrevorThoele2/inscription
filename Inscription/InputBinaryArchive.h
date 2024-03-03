#pragma once

#include "BinaryArchive.h"
#include "InputBinaryFile.h"

#include "Endian.h"

namespace Inscription
{
    class InputBinaryArchive : public BinaryArchive
    {
    public:
        InputBinaryArchive
        (
            const FilePath& path,
            const Signature& signature
        );
        InputBinaryArchive
        (
            const FilePath& path,
            const Signature& signature,
            const TypeRegistrationContext& typeRegistrationContext
        );
        InputBinaryArchive(InputBinaryArchive&& arg) noexcept;

        InputBinaryArchive& operator=(InputBinaryArchive&& arg) noexcept;

        template<class T>
        InputBinaryArchive& Read(T& object);
        template<class T>
        InputBinaryArchive& Construct(T*& object, const std::type_index& type);
    public:
        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
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
        void ReadImpl(Buffer& arg) { ReadFromFile(arg); }
    private:
        InputBinaryFile file;
    private:
        void InitialSetup();
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

    template<class T>
    InputBinaryArchive& InputBinaryArchive::Construct(T*& object, const std::type_index& type)
    {
        DoConstruct(object, type);
        return *this;
    }
}