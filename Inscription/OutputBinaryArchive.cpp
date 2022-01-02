#include "OutputBinaryArchive.h"

namespace Inscription::Archive
{
    OutputBinary::OutputBinary(File::OutputBinary& file) : Binary(Direction::Output), sink(&file)
    {}

    OutputBinary::OutputBinary(Buffer& buffer) : Binary(Direction::Output), sink(&buffer)
    {}
    
    OutputBinary::OutputBinary(OutputBinary&& arg) noexcept :
        Binary(std::move(arg)), sink(std::move(arg.sink))
    {}

    OutputBinary& OutputBinary::operator=(OutputBinary&& arg) noexcept
    {
        Binary::operator=(std::move(arg));
        sink = std::move(arg.sink);
        return *this;
    }
    
    void OutputBinary::PushToBuffer(const Buffer& arg, Buffer& buffer)
    {
        buffer.value.insert(buffer.value.end(), arg.value.begin(), arg.value.end());
    }
}