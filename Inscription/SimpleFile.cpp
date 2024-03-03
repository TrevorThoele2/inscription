
#include "SimpleFile.h"

namespace Inscription
{
    SimpleInFile::SimpleInFile(const Path &path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void SimpleInFile::FillBuffer(char *var, std::streamsize size)
    {
        stream.read(var, size);
    }

    void SimpleInFile::SeekG(StreamT::pos_type pos)
    {
        stream.seekg(pos);
    }

    SimpleOutFile::SimpleOutFile(const Path &path, bool append) : SimpleFile(path, (!append) ? std::ios::out | std::ios::binary : std::ios::out | std::ios::binary | std::ios::app)
    {}

    void SimpleOutFile::SeekP(StreamT::pos_type pos)
    {
        stream.seekp(pos);
    }
}