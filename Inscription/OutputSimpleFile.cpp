#include "OutputSimpleFile.h"

namespace Inscription
{
    OutputSimpleFile::OutputSimpleFile(const Path& path, bool append) :
        SimpleFile(path, (!append) ? std::ios::out | std::ios::binary : std::ios::out | std::ios::binary | std::ios::app)
    {}

    void OutputSimpleFile::SeekP(StreamT::pos_type position)
    {
        stream.seekp(position);
    }
}