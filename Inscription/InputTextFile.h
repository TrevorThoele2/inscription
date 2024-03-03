#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class InputTextFile : public SimpleFile<std::ifstream>
    {
    public:
        InputTextFile(const Path& path);
        InputTextFile& operator>>(std::string& str);
        InputTextFile& operator>>(char& ch);

        std::string GetLine();
        std::string GetLine(char delimiter);
    };
}