#pragma once

#include "TextScribe.h"
#include "OutputTextFile.h"

#include "Path.h"

#include "InvalidScribeDirection.h"

namespace Inscription
{
    class OutputTextScribe : public TextScribe
    {
    public:
        OutputTextScribe(const Path& path, bool append = false);
    protected:
        inline void WriteImpl(const std::string& arg) override { WriteToFile(arg); }
        inline void WriteImpl(const char arg) override { WriteToFile(arg); }

        inline void ReadImpl(std::string& arg) override { ThrowInvalidDirection(); }
        inline void ReadImpl(char& arg) override { ThrowInvalidDirection(); }
        inline void ReadLineImpl(std::string& arg) override { ThrowInvalidDirection(); }
        inline void ReadLineImpl(std::string& arg, char delimiter) override { ThrowInvalidDirection(); }
        inline void ReadSizeImpl(std::string& arg, size_t size) override { ThrowInvalidDirection(); }
    private:
        OutputTextFile file;
    private:
        template<class T>
        inline void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }

        inline void ThrowInvalidDirection() { throw InvalidScribeDirection(); }
    };
}