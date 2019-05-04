
#pragma once

#include <fstream>
#include "Stream.h"
#include "Buffer.h"

namespace Inscription
{
    class BinaryOutputFile : public Stream<std::ofstream>
    {
    public:
        BinaryOutputFile(const Path &path);

        void WriteData(const Buffer &buffer);
        template<class T>
        void WriteData(const T var);

        void SeekStream(StreamPosition pos);
        StreamPosition TellStream();
    };

    template<class T>
    void BinaryOutputFile::WriteData(const T var)
    {
        stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
    }
}