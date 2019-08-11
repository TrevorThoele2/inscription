#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class OutputTextFile : public SimpleFile<std::ofstream>
    {
    public:
        using Width = std::streamsize;
    public:
        OutputTextFile(const FilePath& path, bool append = false);
        OutputTextFile(OutputTextFile&& arg);

        OutputTextFile& operator=(OutputTextFile&& arg);

        void WriteData(const std::string& string);
        void WriteData(const char character);

        void ClearFile();

        // Set this to output strings at a given width.
        // If the string is lower than the width set, it'll fill with this character.
        void SetFillCharacter(const char set);
        void ResetFillCharacter();
        // Set this to output strings at a given width.
        // If the string is lower than this, it'll fill with the fill character.
        void SetWidth(Width set);
        void ResetWidth();
    private:
        char fillCharacter;
        Width width;
    };
}