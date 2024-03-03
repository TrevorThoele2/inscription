#pragma once

#include <fstream>

#include "FilePath.h"
#include "FilePosition.h"
#include "FileSize.h"
#include "SanitizeStreamFailure.h"

#include "Buffer.h"

namespace Inscription::File
{
    class InputBinary final
    {
    public:
        explicit InputBinary(const Path& path);
        InputBinary(InputBinary&& arg) noexcept;

        InputBinary& operator=(InputBinary&& arg) noexcept;

        void ReadData(std::vector<char>& buffer, size_t size);
        template<class T>
        void ReadData(T& var);

        void Seek(Position position);
        void SeekFromBeginning(Position offset = 0);
        void SeekFromEnd(Position offset = 0);
        [[nodiscard]] Position Position();
        [[nodiscard]] Size Size();
    private:
        Path path;
        std::ifstream stream;
        bool failedOpening = false;
    };

    template<class T>
    void InputBinary::ReadData(T& var)
    {
        SanitizeStreamFailure(
            [this, &var] { stream.read(reinterpret_cast<char*>(&var), sizeof(var)); },
            path);
    }
}