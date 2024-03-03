#pragma once

#include <fstream>

#include "SimpleFile.h"

namespace Inscription
{
    class InputSimpleFile : public SimpleFile<std::ifstream>
    {
    public:
        explicit InputSimpleFile(const FilePath& path);

        template<class T>
        InputSimpleFile& operator>>(T& var);

        void FillBuffer(char* var, std::streamsize size);
        void SeekPositionFromCurrent(StreamPosition offset);
        void SeekPositionFromBegin(OffsetPosition offset = 0);
        void SeekPositionFromEnd(OffsetPosition offset = 0);
        [[nodiscard]] StreamPosition CurrentPosition();
    };

    template<class T>
    InputSimpleFile& InputSimpleFile::operator>>(T& var)
    {
        stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        return *this;
    }
}