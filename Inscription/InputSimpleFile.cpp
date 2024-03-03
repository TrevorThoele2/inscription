#include "InputSimpleFile.h"

namespace Inscription
{
    InputSimpleFile::InputSimpleFile(const FilePath& path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void InputSimpleFile::FillBuffer(char* var, std::streamsize size)
    {
        stream.read(var, size);
    }

    void InputSimpleFile::SeekPositionFromCurrent(StreamPosition offset)
    {
        stream.seekg(offset);
    }

    void InputSimpleFile::SeekPositionFromBegin(OffsetPosition offset)
    {
        stream.seekg(offset, std::ios::beg);
    }

    void InputSimpleFile::SeekPositionFromEnd(OffsetPosition offset)
    {
        stream.seekg(offset, std::ios::end);
    }

    auto InputSimpleFile::CurrentPosition() -> StreamPosition
    {
        return stream.tellg();
    }
}