#include "OutputBinaryFile.h"

namespace Inscription
{
    OutputBinaryFile::OutputBinaryFile(const Path& path) : Stream(path, std::ios::out | std::ios::binary)
    {}

    OutputBinaryFile::OutputBinaryFile(OutputBinaryFile&& arg) : Stream(std::move(arg))
    {}

    OutputBinaryFile& OutputBinaryFile::operator=(OutputBinaryFile&& arg)
    {
        Stream::operator=(std::move(arg));
        return *this;
    }

    void OutputBinaryFile::WriteData(const Buffer& buffer)
    {
        stream.write(buffer.value, buffer.size);
    }

    void OutputBinaryFile::SeekStream(StreamPosition position)
    {
        stream.seekp(position);
    }

    OutputBinaryFile::StreamPosition OutputBinaryFile::TellStream()
    {
        return stream.tellp();
    }
}