#pragma once

#include <fstream>

#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class InputBinaryFile : public Stream<std::ifstream>
    {
    public:
        InputBinaryFile(const FilePath& path);
        InputBinaryFile(InputBinaryFile&& arg);

        InputBinaryFile& operator=(InputBinaryFile&& arg);

        void ReadData(Buffer& buffer);
        template<class T>
        void ReadData(T& var);

        void SeekStream(StreamPosition position);
        StreamPosition TellStream();
    };

    template<class T>
    void InputBinaryFile::ReadData(T& var)
    {
        stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        if (stream.bad())
            throw FileEncounteredError();
    }
}