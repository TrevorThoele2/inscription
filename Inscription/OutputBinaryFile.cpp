#include "OutputBinaryFile.h"

namespace Inscription::File
{
    OutputBinary::OutputBinary(const Path& path) :
        path(path), stream(path, std::ios::out | std::ios::binary)
    {}

    OutputBinary::OutputBinary(OutputBinary&& arg) noexcept :
        path(std::move(arg.path)), stream(std::move(arg.stream))
    {}

    OutputBinary& OutputBinary::operator=(OutputBinary&& arg) noexcept
    {
        stream = std::move(arg.stream);
        return *this;
    }

    void OutputBinary::WriteData(const Buffer& buffer)
    {
        SanitizeStreamFailure([this, &buffer]() { stream.write(buffer.value.data(), buffer.value.size()); }, path);
    }

    void OutputBinary::Seek(File::Position position)
    {
        SanitizeStreamFailure([this, position]() { stream.seekp(position); }, path);
    }

    void OutputBinary::SeekFromBeginning(File::Position offset)
    {
        SanitizeStreamFailure([this, offset]() { stream.seekp(offset, std::ofstream::beg); }, path);
    }

    void OutputBinary::SeekFromEnd(File::Position offset)
    {
        SanitizeStreamFailure([this, offset]() { stream.seekp(offset, std::ofstream::end); }, path);
    }

    Position OutputBinary::Position()
    {
        return SanitizeStreamFailure<File::Position>([this]() { return stream.tellp(); }, path);
    }
}