
#pragma once

#include <fstream>
#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class BinaryInputFile : public Stream<std::ifstream>
    {
    public:
        BinaryInputFile(const Path &path);

        void ReadData(Buffer &buffer);
        template<class T>
        void ReadData(T &var);

        void SeekStream(StreamPos pos);
        StreamPos TellStream();
    };

    template<class T>
    void BinaryInputFile::ReadData(T &var)
    {
        stream.read(reinterpret_cast<char*>(&var), sizeof(var));
    }
}