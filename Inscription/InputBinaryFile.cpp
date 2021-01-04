#include "InputBinaryFile.h"

namespace Inscription
{
    InputBinaryFile::InputBinaryFile(const FilePath& path) : Stream(path, std::ios::in | std::ios::binary)
    {}

    InputBinaryFile::InputBinaryFile(InputBinaryFile&& arg) noexcept : Stream(std::move(arg))
    {}

    InputBinaryFile& InputBinaryFile::operator=(InputBinaryFile&& arg) noexcept
    {
        Stream::operator=(std::move(arg));
        return *this;
    }

    void InputBinaryFile::ReadData(std::vector<char>& buffer, size_t size)
    {
        if (buffer.empty())
            throw FileEncounteredError();

        stream.read(&buffer[0], size);
        if (FailedStream())
            throw FileEncounteredError();
    }

    void InputBinaryFile::SeekStream(StreamPosition offset)
    {
        stream.seekg(offset);
        if (FailedStream())
            throw FileEncounteredError();
    }

    void InputBinaryFile::SeekStreamFromBegin(StreamPosition offset)
    {
        stream.seekg(offset, std::ifstream::beg);
        if (FailedStream())
            throw FileEncounteredError();
    }

    void InputBinaryFile::SeekStreamFromEnd(StreamPosition offset)
    {
        stream.seekg(offset, std::ifstream::end);
        if (FailedStream())
            throw FileEncounteredError();
    }

    InputBinaryFile::StreamPosition InputBinaryFile::TellStream()
    {
        const auto told = stream.tellg();
        if (FailedStream())
            throw FileEncounteredError();
        return told;
    }

    auto InputBinaryFile::Size() -> SizeT
    {
        const auto currentPosition = TellStream();
        stream.ignore(std::numeric_limits<SizeT>::max());
        const auto size = stream.gcount();
        stream.clear();
        SeekStream(currentPosition);
        return size;
    }
}