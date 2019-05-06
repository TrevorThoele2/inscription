
#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class OutputTextFile : public SimpleFile<std::ofstream>
    {
    public:
        typedef std::streamsize Width;
    public:
        OutputTextFile(const Path& path, bool append = false);

        OutputTextFile& operator<<(const std::string& str);
        OutputTextFile& operator<<(const char ch);

        void ClearFile();

        // Set this to output strings at a given width. If the string is lower than the width set, it'll fill with this character.
        void SetFillCharacter(const char set);
        void ResetFillCharacter();
        // Set this to output strings at a given width. If the string is lower than this, it'll fill with the fill character.
        void SetWidth(Width set);
        void ResetWidth();
    private:
        char fillChar;
        Width width;
    };
}