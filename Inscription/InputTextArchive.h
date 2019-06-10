#pragma once

#include "TextArchive.h"
#include "InputTextFile.h"

#include "InvalidArchiveDirection.h"

namespace Inscription
{
    class InputTextArchive : public TextArchive
    {
    public:
        InputTextArchive(const Path& path);
    protected:
        inline void WriteImpl(const std::string& arg) override { ThrowInvalidDirection(); }
        inline void WriteImpl(const char arg) override { ThrowInvalidDirection(); }

        inline void ReadImpl(std::string& arg) override { ReadFromFile(arg); }
        inline void ReadImpl(char& arg) override { ReadFromFile(arg); }
        inline void ReadLineImpl(std::string& arg) override { ReadLineFromFile(arg); }
        inline void ReadLineImpl(std::string& arg, char delimiter) override { ReadLineFromFile(arg, delimiter); }
        inline void ReadSizeImpl(std::string& arg, size_t size) override { ReadSizeFromFile(arg, size); }
    private:
        InputTextFile file;
    private:
        template<class T>
        inline void ReadFromFile(T& arg)
        {
            file.ReadData(arg);
        }

        inline void ReadLineFromFile(std::string& arg)
        {
            arg = file.ReadLine();
        }

        inline void ReadLineFromFile(std::string& arg, char delimiter)
        {
            arg = file.ReadLine(delimiter);
        }

        inline void ReadSizeFromFile(std::string& arg, size_t size)
        {
            arg = file.ReadSize(size);
        }

        inline void ThrowInvalidDirection() { throw InvalidArchiveDirection(); }
    };
}