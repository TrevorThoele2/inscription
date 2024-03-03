#pragma once

#include "TextArchive.h"
#include "OutputTextFile.h"

namespace Inscription
{
    class OutputTextArchive : public TextArchive
    {
    public:
        explicit OutputTextArchive(const FilePath& path, bool append = false);
        OutputTextArchive(const OutputTextArchive& arg) = delete;
        OutputTextArchive(OutputTextArchive&& arg) noexcept;

        ~OutputTextArchive() = default;

        OutputTextArchive& operator=(const OutputTextArchive& arg) = delete;
        OutputTextArchive& operator=(OutputTextArchive&& arg) noexcept;

        OutputTextArchive& Write(const std::string& arg);
        OutputTextArchive& Write(const char arg);
    protected:
        void WriteImpl(const std::string& arg) { WriteToFile(arg); }
        void WriteImpl(const char arg) { WriteToFile(arg); }
    private:
        OutputTextFile file;
    private:
        template<class T>
        void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };
}