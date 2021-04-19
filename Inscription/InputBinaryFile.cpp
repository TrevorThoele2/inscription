#include "InputBinaryFile.h"

namespace Inscription::File
{
    InputBinary::InputBinary(const Path& path) : path(path)
    {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
        SanitizeStreamFailure([this, path]() { stream.open(path, std::ios::in | std::ios::binary); }, path);
    }

    InputBinary::InputBinary(InputBinary&& arg) noexcept :
        path(std::move(arg.path)), stream(std::move(arg.stream))
    {}

    InputBinary& InputBinary::operator=(InputBinary&& arg) noexcept
    {
        path = std::move(arg.path);
        stream = std::move(arg.stream);
        return *this;
    }

    void InputBinary::ReadData(std::vector<char>& buffer, size_t size)
    {
        SanitizeStreamFailure(
            [this, &buffer, size]()
            {
                buffer.resize(size);
                stream.read(&buffer[0], size);
            },
            path);
    }

    void InputBinary::Seek(File::Position position)
    {
        SanitizeStreamFailure(
            [this, position]() { stream.seekg(position); },
            path);
    }

    void InputBinary::SeekFromBeginning(File::Position offset)
    {
        SanitizeStreamFailure(
            [this, offset]() { stream.seekg(offset, std::ifstream::beg); },
            path);
    }

    void InputBinary::SeekFromEnd(File::Position offset)
    {
        SanitizeStreamFailure(
            [this, offset]() { stream.seekg(offset, std::ifstream::end); },
            path);
    }

    Position InputBinary::Position()
    {
        return SanitizeStreamFailure<std::ios::pos_type>(
            [this]() { return stream.tellg(); },
            path);
    }

    Size InputBinary::Size()
    {
        return SanitizeStreamFailure<File::Size>(
            [this]()
            {
                const auto currentPosition = stream.tellg();
                stream.ignore(std::numeric_limits<File::Size>::max());
                const auto size = stream.gcount();
                stream.clear();
                Seek(currentPosition);
                return size;
            },
            path);
    }
}