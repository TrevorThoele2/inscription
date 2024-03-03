
#include "InputBinaryFile.h"

namespace Inscription
{
    InputBinaryFile::InputBinaryFile(const Path &path) : Stream(path, std::ios::in | std::ios::binary)
    {}

    void InputBinaryFile::ReadData(Buffer &buffer)
    {
        stream.read(buffer.value, buffer.size);
    }

    void InputBinaryFile::SeekStream(StreamPosition pos)
    {
        stream.seekg(pos);
    }

    InputBinaryFile::StreamPosition InputBinaryFile::TellStream()
    {
        return stream.tellg();
    }
}