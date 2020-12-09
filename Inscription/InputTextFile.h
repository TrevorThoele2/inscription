#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class InputTextFile final : public SimpleFile<std::ifstream>
    {
    public:
        explicit InputTextFile(const FilePath& path);
        InputTextFile(InputTextFile&& arg) noexcept;

        InputTextFile& operator=(InputTextFile&& arg) noexcept;

        void ReadData(std::string& string);
        void ReadData(char& character);

        std::string ReadLine();
        std::string ReadLine(char delimiter);

        std::string ReadSize(size_t size);

        void SeekStream(StreamPosition position);
        [[nodiscard]] StreamPosition TellStream();
    };
}