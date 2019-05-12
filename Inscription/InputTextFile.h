#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class InputTextFile : public SimpleFile<std::ifstream>
    {
    public:
        InputTextFile(const Path& path);

        void ReadData(std::string& string);
        void ReadData(char& character);

        std::string ReadLine();
        std::string ReadLine(char delimiter);

        std::string ReadSize(size_t size);
    };
}