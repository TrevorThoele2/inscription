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

    void InputBinaryFile::ReadData(Buffer& buffer)
    {
        stream.read(buffer.value, buffer.size);
        if (FailedStream())
            throw FileEncounteredError();
    }

    void InputBinaryFile::SeekStream(StreamPosition position)
    {
        stream.seekg(position);
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
}