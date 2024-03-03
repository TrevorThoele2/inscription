#pragma once

#include "BinaryScribe.h"
#include "OutputBinaryFile.h"

#include "InvalidScribeDirection.h"

namespace Inscription
{
    class OutputBinaryScribe : public BinaryScribe
    {
    public:
        OutputBinaryScribe(const Path& path, const Signature& signature, Version version);
        OutputBinaryScribe(const Path& path, const Signature& signature, Version version, const TypeRegistrationContext& typeRegistrationContext);
        OutputBinaryScribe(OutputBinaryScribe&& arg);

        OutputBinaryScribe& operator=(OutputBinaryScribe&& arg);
    public:
        template<class T>
        inline OutputBinaryScribe& Save(T&& arg);

        /*
        Call this if the pointer owns its resource
        Creates the resource
        */
        template<class T>
        inline OutputBinaryScribe& SaveOwningPointer(T* arg);

        /*
        Call this if the pointer does not own its own resource
        Never creates the resource
        */
        template<class T>
        inline OutputBinaryScribe& SaveUnowningPointer(T* arg);

        template<class T>
        void WriteNumeric(T arg);

        void WriteBuffer(const Buffer& write);
    public:
        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
    protected:
        inline void WriteImpl(bool arg) override { WriteToFile(arg); }
        inline void WriteImpl(signed char arg) override { WriteToFile(arg); }
        inline void WriteImpl(char arg) override { WriteToFile(arg); }
        inline void WriteImpl(short arg) override { WriteToFile(arg); }
        inline void WriteImpl(int arg) override { WriteToFile(arg); }
        inline void WriteImpl(long arg) override { WriteToFile(arg); }
        inline void WriteImpl(long long arg) override { WriteToFile(arg); }
        inline void WriteImpl(unsigned char arg) override { WriteToFile(arg); }
        inline void WriteImpl(unsigned short arg) override { WriteToFile(arg); }
        inline void WriteImpl(unsigned int arg) override { WriteToFile(arg); }
        inline void WriteImpl(unsigned long arg) override { WriteToFile(arg); }
        inline void WriteImpl(unsigned long long arg) override { WriteToFile(arg); }
        inline void WriteImpl(float arg) override { WriteToFile(arg); }
        inline void WriteImpl(double arg) override { WriteToFile(arg); }
        inline void WriteImpl(const Buffer &arg) override { WriteToFile(arg); }

        inline void ReadImpl(bool& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(signed char& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(char& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(short& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(int& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(long& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(long long& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(unsigned char& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(unsigned short& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(unsigned int& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(unsigned long& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(unsigned long long&  arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(float& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(double& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(Buffer& arg) override { ThrowInvalidDirection(); }
    private:
        OutputBinaryFile file;
    private:
        void InitialSetup();
    private:
        template<class T>
        inline void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }

        inline void ThrowInvalidDirection() { throw InvalidScribeDirection(); }
    };

    template<class T>
    OutputBinaryScribe& OutputBinaryScribe::Save(T&& arg)
    {
        ProcessSave(arg);
        return *this;
    }

    template<class T>
    OutputBinaryScribe& OutputBinaryScribe::SaveOwningPointer(T* arg)
    {
        ProcessSaveOwningPointerImpl(arg);
        return *this;
    }

    template<class T>
    OutputBinaryScribe& OutputBinaryScribe::SaveUnowningPointer(T* arg)
    {
        ProcessSaveUnowningPointerImpl(arg);
        return *this;
    }

    template<class T>
    void OutputBinaryScribe::WriteNumeric(T arg)
    {
        WriteNumericImplementation(arg);
    }
}