
#pragma once

#include <fstream>
#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class InputBinaryFile : public Stream<std::ifstream>
    {
    public:
        InputBinaryFile(const Path &path);

        void ReadData(Buffer &buffer);
        template<class T>
        void ReadData(T &var);

        void SeekStream(StreamPosition pos);
        StreamPosition TellStream();
    };

    template<class T>
    void InputBinaryFile::ReadData(T &var)
    {
        stream.read(reinterpret_cast<char*>(&var), sizeof(var));
    }
}