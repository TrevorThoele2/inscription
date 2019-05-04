
#pragma once

#include "Scribe.h"
#include "BinaryOutputFile.h"
#include "BinaryInputFile.h"

namespace Inscription
{
    class BinaryScribeMarkerException : public Exception
    {
    public:
        BinaryScribeMarkerException();
    };

    // Basic binary scribe
    class BinaryScribe : public Scribe
    {
    public:
        typedef std::string Marker;
    public:
        // Output constructor
        BinaryScribe(const Path& path, const Marker& marker, Version version);
        // Input constructor
        BinaryScribe(const Path& path, const Marker& marker);
        BinaryScribe(BinaryScribe&& arg);
        BinaryScribe& operator=(BinaryScribe&& arg);
        ~BinaryScribe();

        const Marker& GetMarker() const;
        Version GetVersion() const override;

        void SeekStream(StreamPosition pos) override;
        StreamPosition TellStream() override;
    private:
        typedef BinaryOutputFile OutputFileT;
        typedef BinaryInputFile InputFileT;

        union FileT
        {
            OutputFileT* out;
            InputFileT* in;
        };
    private:
        FileT file;
        Marker marker;
        Version version;

        StreamPosition postHeaderPosition;

        BinaryScribe(const BinaryScribe& arg) = delete;
        BinaryScribe& operator=(const BinaryScribe& arg) = delete;

        StreamPosition GetPostHeaderPosition() const override;

        inline void WriteImpl(bool arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(signed char arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(char arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(short arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(int arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(long arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(long long arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(unsigned char arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(unsigned short arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(unsigned int arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(unsigned long arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(unsigned long long arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(float arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(double arg) override { BinaryScribe::WriteNumericImpl(arg); }
        inline void WriteImpl(const Buffer &arg) override { file.out->WriteData(arg); }

        inline void ReadImpl(bool& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(signed char& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(char& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(short& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(int& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(long& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(long long& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(unsigned char& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(unsigned short& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(unsigned int& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(unsigned long& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(unsigned long long&  arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(float& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(double& arg) override { BinaryScribe::ReadNumericImpl(arg); }
        inline void ReadImpl(Buffer& arg) override { file.in->ReadData(arg); }

        template<class T>
        inline void WriteNumericImpl(T& arg)
        {
            file.out->WriteData(arg);
        }

        template<class T>
        inline void ReadNumericImpl(T& arg)
        {
            file.in->ReadData(arg);
        }
    };
}