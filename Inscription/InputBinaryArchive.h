#pragma once

#include "BinaryArchive.h"
#include "InputBinaryFile.h"

#include "InvalidArchiveDirection.h"

namespace Inscription
{
    class InputBinaryArchive : public BinaryArchive
    {
    public:
        InputBinaryArchive(const Path& path, const Signature& signature);
        InputBinaryArchive(const Path& path, const Signature& signature, const TypeRegistrationContext& typeRegistrationContext);
        InputBinaryArchive(InputBinaryArchive&& arg);

        InputBinaryArchive& operator=(InputBinaryArchive&& arg);

        template<class T>
        InputBinaryArchive& ReadNumeric(T& object);
    public:
        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
    protected:
        inline void ReadImpl(bool& arg) { ReadFromFile(arg); }
        inline void ReadImpl(signed char& arg) { ReadFromFile(arg); }
        inline void ReadImpl(char& arg) { ReadFromFile(arg); }
        inline void ReadImpl(short& arg) { ReadFromFile(arg); }
        inline void ReadImpl(int& arg) { ReadFromFile(arg); }
        inline void ReadImpl(long& arg) { ReadFromFile(arg); }
        inline void ReadImpl(long long& arg) { ReadFromFile(arg); }
        inline void ReadImpl(unsigned char& arg) { ReadFromFile(arg); }
        inline void ReadImpl(unsigned short& arg) { ReadFromFile(arg); }
        inline void ReadImpl(unsigned int& arg) { ReadFromFile(arg); }
        inline void ReadImpl(unsigned long& arg) { ReadFromFile(arg); }
        inline void ReadImpl(unsigned long long&  arg) { ReadFromFile(arg); }
        inline void ReadImpl(float& arg) { ReadFromFile(arg); }
        inline void ReadImpl(double& arg) { ReadFromFile(arg); }
        inline void ReadImpl(Buffer& arg) { ReadFromFile(arg); }
    private:
        InputBinaryFile file;
    private:
        void InitialSetup();
    private:
        template<class T>
        inline void ReadFromFile(T& arg)
        {
            file.ReadData(arg);
        }
    };

    template<class T>
    InputBinaryArchive& InputBinaryArchive::ReadNumeric(T& object)
    {
        static_assert(std::is_arithmetic_v<T>, "The T given to ReadNumeric was not arithmetic.");

#ifdef INSCRIPTION_COMMON_NUMERICS
        ReadImpl(reinterpret_cast<typename NumericTraits<T>::Size&>(object));
#else
        ReadImpl(object);
#endif

        if (!IsLittleEndian())
            ByteSwap(object);

        return *this;
    }
}