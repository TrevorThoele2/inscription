
#include "BinaryOutputFile.h"

namespace Inscription
{
    BinaryOutputFile::BinaryOutputFile(const Path &path) : Stream(path, std::ios::out | std::ios::binary)
    {}

    void BinaryOutputFile::WriteData(const Buffer &buffer)
    {
        stream.write(buffer.value, buffer.size);
    }

    void BinaryOutputFile::SeekStream(StreamPosition pos)
    {
        stream.seekp(pos);
    }

    BinaryOutputFile::StreamPosition BinaryOutputFile::TellStream()
    {
        return stream.tellp();
    }
}