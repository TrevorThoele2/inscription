#pragma once

#include "TextArchive.h"
#include "OutputTextFile.h"

#include "InvalidArchiveDirection.h"

namespace Inscription
{
    class OutputTextArchive : public TextArchive
    {
    public:
        OutputTextArchive(const FilePath& path, bool append = false);
        OutputTextArchive(OutputTextArchive&& arg);

        OutputTextArchive& operator=(OutputTextArchive&& arg);

        OutputTextArchive& Write(const std::string& arg);
        OutputTextArchive& Write(const char arg);
    protected:
        inline void WriteImpl(const std::string& arg) { WriteToFile(arg); }
        inline void WriteImpl(const char arg) { WriteToFile(arg); }
    private:
        OutputTextFile file;
    private:
        template<class T>
        inline void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };
}