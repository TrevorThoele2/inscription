#include "OutputSimpleFile.h"

namespace Inscription
{
    OutputSimpleFile::OutputSimpleFile(const FilePath& path, bool append) :
        SimpleFile(path, (!append) ? std::ios::out | std::ios::binary : std::ios::out | std::ios::binary | std::ios::app)
    {}

    void OutputSimpleFile::Seek(StreamT::pos_type position)
    {
        stream.seekp(position);
    }

    void OutputSimpleFile::Seek(OffsetPosition position, std::ios_base::seekdir direction)
    {
        stream.seekp(position, direction);
    }

    auto OutputSimpleFile::Tell() -> StreamT::pos_type
    {
        return stream.tellp();
    }
}