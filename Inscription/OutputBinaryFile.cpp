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
        stream.write(buffer.value.data(), buffer.value.size());
        if (FailedStream())
            throw FileEncounteredError(Path());
    }

    void OutputBinaryFile::SeekStreamFromCurrent(StreamPosition offset)
    {
        stream.seekp(offset);
        if (FailedStream())
            throw FileEncounteredError(Path());
    }

    void OutputBinaryFile::SeekStreamFromBegin(StreamPosition offset)
    {
        stream.seekp(offset, std::ofstream::beg);
        if (FailedStream())
            throw FileEncounteredError(Path());
    }

    void OutputBinaryFile::SeekStreamFromEnd(StreamPosition offset)
    {
        stream.seekp(offset, std::ofstream::end);
        if (FailedStream())
            throw FileEncounteredError(Path());
    }

    OutputBinaryFile::StreamPosition OutputBinaryFile::TellStream()
    {
        const auto told = stream.tellp();
        if (FailedStream())
            throw FileEncounteredError(Path());
        return told;
    }
}