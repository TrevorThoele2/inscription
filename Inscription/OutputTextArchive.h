#pragma once

#include "TextArchive.h"
#include "OutputTextFile.h"

namespace Inscription::Archive
{
    class OutputText : public Text
    {
    public:
        explicit OutputText(const File::Path& path, bool append = false);
        OutputText(const OutputText& arg) = delete;
        OutputText(OutputText&& arg) noexcept;
        
        OutputText& operator=(const OutputText& arg) = delete;
        OutputText& operator=(OutputText&& arg) noexcept;

        OutputText& Write(const std::string& arg);
        OutputText& Write(const char arg);
    protected:
        void WriteImpl(const std::string& arg) { WriteToFile(arg); }
        void WriteImpl(const char arg) { WriteToFile(arg); }
    private:
       File::OutputText file;
    private:
        template<class T>
        void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };
}