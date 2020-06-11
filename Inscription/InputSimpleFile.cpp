#include "InputSimpleFile.h"

namespace Inscription
{
    InputSimpleFile::InputSimpleFile(const FilePath& path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void InputSimpleFile::FillBuffer(char* var, std::streamsize size)
    {
        stream.read(var, size);
    }

    void InputSimpleFile::Seek(StreamPosition position)
    {
        stream.seekg(position);
    }

    void InputSimpleFile::Seek(OffsetPosition position, std::ios_base::seekdir direction)
    {
        stream.seekg(position, direction);
    }

    auto InputSimpleFile::Tell() -> StreamPosition
    {
        return stream.tellg();
    }
}