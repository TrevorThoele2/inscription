#pragma once

#include "BinaryScribe.h"
#include "InputBinaryFile.h"

#include "InvalidScribeDirection.h"

namespace Inscription
{
    class InputBinaryScribe : public BinaryScribe
    {
    public:
        InputBinaryScribe(const Path& path, const Signature& signature);
        InputBinaryScribe(InputBinaryScribe&& arg);

        InputBinaryScribe& operator=(InputBinaryScribe&& arg);

        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
    protected:
        inline void WriteImpl(bool arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(signed char arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(char arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(short arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(int arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(long arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(long long arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(unsigned char arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(unsigned short arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(unsigned int arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(unsigned long arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(unsigned long long arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(float arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(double arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(const Buffer &arg) override { ThrowInvalidDirection(); }

        inline void ReadImpl(bool& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(signed char& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(char& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(short& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(int& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(long& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(long long& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(unsigned char& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(unsigned short& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(unsigned int& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(unsigned long& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(unsigned long long&  arg) override { ReadFromFile(arg); }
        inline void ReadImpl(float& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(double& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(Buffer& arg) override { ReadFromFile(arg); }
    private:
        InputBinaryFile file;
    private:
        template<class T>
        inline void ReadFromFile(T& arg)
        {
            file.ReadData(arg);
        }

        inline void ThrowInvalidDirection() { throw InvalidScribeDirection(); }
    };
}