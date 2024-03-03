
#pragma once

#include "SimpleFile.h"

namespace inscription
{
    class TextInFile : public SimpleFile<std::ifstream>
    {
    public:
        TextInFile(const std::string &name);

        void GetLine(std::string &str);
        void GetLine(std::string &str, char delimiter);
    };

    class TextOutFile : public SimpleFile<std::ofstream>
    {
    public: typedef std::streamsize Width;
    public:
        TextOutFile(const std::string &name, bool append = false);
        TextOutFile& operator<<(const std::string &str);
        TextOutFile& operator<<(const char ch);
        void Flush();
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