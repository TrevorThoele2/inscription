#include "OutputSimpleFile.h"

namespace Inscription
{
    OutputSimpleFile::OutputSimpleFile(const FilePath& path, bool append) :
        SimpleFile(path, (!append) ? std::ios::out | std::ios::binary : std::ios::out | std::ios::binary | std::ios::app)
    {}

    void OutputSimpleFile::SeekPositionFromCurrent(StreamT::pos_type offset)
    {
        stream.seekp(offset);
    }

    void OutputSimpleFile::SeekPositionFromBegin(OffsetPosition offset)
    {
        stream.seekp(offset, std::ios::beg);
    }

    void OutputSimpleFile::SeekPositionFromEnd(OffsetPosition offset)
    {
        stream.seekp(offset, std::ios::end);
    }

    auto OutputSimpleFile::CurrentPosition() -> StreamT::pos_type
    {
        return stream.tellp();
    }
}