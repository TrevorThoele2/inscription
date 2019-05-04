
#include "BinaryInputFile.h"

namespace Inscription
{
    BinaryInputFile::BinaryInputFile(const Path &path) : Stream(path, std::ios::in | std::ios::binary)
    {}

    void BinaryInputFile::ReadData(Buffer &buffer)
    {
        stream.read(buffer.value, buffer.size);
    }

    void BinaryInputFile::SeekStream(StreamPosition pos)
    {
        stream.seekg(pos);
    }

    BinaryInputFile::StreamPosition BinaryInputFile::TellStream()
    {
        return stream.tellg();
    }
}