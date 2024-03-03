
#include "BinaryOutputFile.h"

namespace inscription
{
    BinaryOutputFile::BinaryOutputFile(const Path &path) : Stream(path, std::ios::out | std::ios::binary)
    {}

    void BinaryOutputFile::WriteData(const Buffer &buffer)
    {
        stream.write(buffer.value, buffer.size);
    }

    void BinaryOutputFile::SeekStream(StreamPos pos)
    {
        stream.seekp(pos);
    }

    BinaryOutputFile::StreamPos BinaryOutputFile::TellStream()
    {
        return stream.tellp();
    }
}