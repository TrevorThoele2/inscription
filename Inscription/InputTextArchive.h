#pragma once

#include <variant>
#include <sstream>
#include "TextArchive.h"
#include "InputTextFile.h"

namespace Inscription::Archive
{
    class InputText final : public Text
    {
    public:
        explicit InputText(File::InputText& file);
        explicit InputText(std::string& string);
        InputText(const InputText& arg) = delete;
        InputText(InputText&& arg) noexcept;
        
        InputText& operator=(const InputText& arg) = delete;
        InputText& operator=(InputText&& arg) noexcept;
        
        InputText& ReadUntil(std::string& arg, char delimiter);
        InputText& ReadSize(std::string& arg, size_t size = std::numeric_limits<size_t>::max());
    private:
        struct StringSource
        {
            size_t position;
            std::string string;
        };

        using Source = std::variant<File::InputText*, StringSource>;
        Source source;
    };
}