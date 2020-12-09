#pragma once

#include <fstream>
#include "SimpleFile.h"

namespace Inscription
{
    class OutputTextFile final : public SimpleFile<std::ofstream>
    {
    public:
        using Width = std::streamsize;
    public:
        explicit OutputTextFile(const FilePath& path, bool append = false);
        OutputTextFile(OutputTextFile&& arg) noexcept;

        OutputTextFile& operator=(OutputTextFile&& arg) noexcept;

        void WriteData(const std::string& string);
        void WriteData(char character);

        void ClearFile();

        // Set this to output strings at a given width.
        // If the string is lower than the width set, it'll fill with this character.
        void SetFillCharacter(char set);
        void ResetFillCharacter();
        // Set this to output strings at a given width.
        // If the string is lower than this, it'll fill with the fill character.
        void SetWidth(Width set);
        void ResetWidth();

        void SeekStream(StreamPosition position);
        [[nodiscard]] StreamPosition TellStream();
    private:
        char fillCharacter = ' ';
        Width width = 0;
    };
}