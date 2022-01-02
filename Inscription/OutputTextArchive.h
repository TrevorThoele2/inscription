#pragma once

#include <sstream>
#include <variant>
#include "TextArchive.h"
#include "OutputTextFile.h"

namespace Inscription::Archive
{
    class OutputText final : public Text
    {
    public:
        explicit OutputText(File::OutputText& file);
        explicit OutputText(std::string& string);
        OutputText(const OutputText& arg) = delete;
        OutputText(OutputText&& arg) noexcept;
        
        OutputText& operator=(const OutputText& arg) = delete;
        OutputText& operator=(OutputText&& arg) noexcept;

        OutputText& Write(const std::string& arg);
        OutputText& Write(const char arg);
    private:
        std::variant<File::OutputText*, std::string*> source;
    };
}