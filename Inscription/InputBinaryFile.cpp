#include "InputBinaryFile.h"

namespace Inscription
{
    InputBinaryFile::InputBinaryFile(const Path& path) : Stream(path, std::ios::in | std::ios::binary)
    {}

    InputBinaryFile::InputBinaryFile(InputBinaryFile&& arg) : Stream(std::move(arg))
    {}

    InputBinaryFile& InputBinaryFile::operator=(InputBinaryFile&& arg)
    {
        Stream::operator=(std::move(arg));
        return *this;
    }

    void InputBinaryFile::ReadData(Buffer& buffer)
    {
        stream.read(buffer.value, buffer.size);
    }

    void InputBinaryFile::SeekStream(StreamPosition position)
    {
        stream.seekg(position);
    }

    InputBinaryFile::StreamPosition InputBinaryFile::TellStream()
    {
        return stream.tellg();
    }
}