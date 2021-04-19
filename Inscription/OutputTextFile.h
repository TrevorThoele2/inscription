#pragma once

#include <fstream>

#include "FilePath.h"
#include "FilePosition.h"
#include "SanitizeStreamFailure.h"

namespace Inscription::File
{
    class OutputText final
    {
    public:
        using Width = std::streamsize;
    public:
        explicit OutputText(const Path& path, bool append = false);
        OutputText(OutputText&& arg) noexcept;

        OutputText& operator=(OutputText&& arg) noexcept;

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

        void Seek(Position position);
        [[nodiscard]] Position Position();
    private:
        Path path;
        std::ofstream stream;

        char fillCharacter = ' ';
        Width width = 0;
    };
}