
#include "OutputBinaryFile.h"

namespace Inscription
{
    OutputBinaryFile::OutputBinaryFile(const Path &path) : Stream(path, std::ios::out | std::ios::binary)
    {}

    void OutputBinaryFile::WriteData(const Buffer &buffer)
    {
        stream.write(buffer.value, buffer.size);
    }

    void OutputBinaryFile::SeekStream(StreamPosition pos)
    {
        stream.seekp(pos);
    }

    OutputBinaryFile::StreamPosition OutputBinaryFile::TellStream()
    {
        return stream.tellp();
    }
}