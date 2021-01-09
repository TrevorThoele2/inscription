#include "InputTextFile.h"

namespace Inscription
{
    InputTextFile::InputTextFile(const FilePath& path) : SimpleFile(path, std::ios::in)
    {}

    InputTextFile::InputTextFile(InputTextFile&& arg) noexcept : SimpleFile(std::move(arg))
    {}

    InputTextFile& InputTextFile::operator=(InputTextFile&& arg) noexcept
    {
        SimpleFile::operator=(std::move(arg));
        return *this;
    }

    void InputTextFile::ReadData(std::string& string)
    {
        stream >> string;
    }

    void InputTextFile::ReadData(char& character)
    {
        stream >> character;
    }

    std::string InputTextFile::ReadLine()
    {
        std::string string;
        std::getline(stream, string);
        return string;
    }

    std::string InputTextFile::ReadLine(char delimiter)
    {
        std::string string;
        std::getline(stream, string, delimiter);
        return string;
    }

    std::string InputTextFile::ReadSize(size_t size)
    {
        const auto startPosition = stream.tellg();

        stream.ignore(std::numeric_limits<std::streamsize>::max());
        const auto useSize = stream.gcount();
        stream.clear();
        stream.seekg(startPosition, std::ios_base::beg);

        std::string buffer;
        buffer.resize(static_cast<size_t>(useSize));

        stream.read(&buffer[0], useSize);

        while (buffer[buffer.size() - 1] == '\0')
            buffer.erase(buffer.size() - 1);

        return buffer;
    }

    void InputTextFile::SeekStream(StreamPosition position)
    {
        stream.seekg(position);
    }

    StreamPosition InputTextFile::TellStream()
    {
        return stream.tellg();
    }
}