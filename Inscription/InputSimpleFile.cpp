
#include "InputSimpleFile.h"

namespace Inscription
{
    InputSimpleFile::InputSimpleFile(const Path& path) : SimpleFile(path, std::ios::in | std::ios::binary)
    {}

    void InputSimpleFile::FillBuffer(char* var, std::streamsize size)
    {
        stream.read(var, size);
    }

    void InputSimpleFile::SeekG(StreamT::pos_type position)
    {
        stream.seekg(position);
    }
}