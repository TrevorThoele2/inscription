#pragma once

#include <fstream>

#include "FilePath.h"
#include "FilePosition.h"
#include "SanitizeStreamFailure.h"

#include "Buffer.h"

namespace Inscription::File
{
    class OutputBinary final
    {
    public:
        explicit OutputBinary(const Path& path);
        OutputBinary(OutputBinary&& arg) noexcept;

        OutputBinary& operator=(OutputBinary&& arg) noexcept;

        void WriteData(const Buffer& buffer);
        template<class T>
        void WriteData(T var);

        void Seek(Position position);
        void SeekFromBeginning(Position offset = 0);
        void SeekFromEnd(Position offset = 0);
        [[nodiscard]] Position Position();
    private:
        Path path;
        std::ofstream stream;
    };

    template<class T>
    void OutputBinary::WriteData(T var)
    {
        SanitizeStreamFailure([this, &var]() { stream.write(reinterpret_cast<const char*>(&var), sizeof(var)); }, path);
    }
}