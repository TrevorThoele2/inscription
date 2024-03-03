
#include "BinaryInputFile.h"

namespace inscription
{
    BinaryInputFile::BinaryInputFile(const Path &path) : Stream(path, std::ios::in | std::ios::binary)
    {}

    void BinaryInputFile::ReadData(Buffer &buffer)
    {
        stream.read(buffer.value, buffer.size);
    }

    void BinaryInputFile::SeekStream(StreamPos pos)
    {
        stream.seekg(pos);
    }

    BinaryInputFile::StreamPos BinaryInputFile::TellStream()
    {
        return stream.tellg();
    }
}