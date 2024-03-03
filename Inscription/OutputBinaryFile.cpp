#include "OutputBinaryFile.h"

namespace Inscription
{
    OutputBinaryFile::OutputBinaryFile(const FilePath& path) : Stream(path, std::ios::out | std::ios::binary)
    {}

    OutputBinaryFile::OutputBinaryFile(OutputBinaryFile&& arg) noexcept : Stream(std::move(arg))
    {}

    OutputBinaryFile& OutputBinaryFile::operator=(OutputBinaryFile&& arg) noexcept
    {
        Stream::operator=(std::move(arg));
        return *this;
    }

    void OutputBinaryFile::WriteData(const Buffer& buffer)
    {
        stream.write(buffer.data(), buffer.size());
        if (FailedStream())
            throw FileEncounteredError();
    }

    void OutputBinaryFile::SeekStream(StreamPosition position)
    {
        stream.seekp(position);
        if (FailedStream())
            throw FileEncounteredError();
    }

    OutputBinaryFile::StreamPosition OutputBinaryFile::TellStream()
    {
        const auto told = stream.tellp();
        if (FailedStream())
            throw FileEncounteredError();
        return told;
    }
}