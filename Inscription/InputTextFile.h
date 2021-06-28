#pragma once

#include <fstream>

#include "FilePath.h"
#include "FilePosition.h"
#include "SanitizeStreamFailure.h"

namespace Inscription::File
{
    class InputText final
    {
    public:
        explicit InputText(const Path& path);
        InputText(InputText&& arg) noexcept;

        InputText& operator=(InputText&& arg) noexcept;

        void ReadData(std::string& string);
        void ReadData(char& character);
        std::string ReadLine();
        std::string ReadLine(char delimiter);
        std::string ReadSize(size_t size);

        void Seek(Position position);
        [[nodiscard]] Position Position();

        [[nodiscard]] bool IsAtEnd() const;
    private:
        Path path;
        std::ifstream stream;
        bool failedOpening = false;
    };
}