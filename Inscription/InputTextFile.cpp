#include "InputTextFile.h"

namespace Inscription::File
{
    InputText::InputText(const Path& path) : path(path)
    {
        stream.exceptions(std::ios::failbit | std::ios::badbit);
        SanitizeStreamFailure([this, path]() {stream.open(path, std::ios::in); }, path);
    }

    InputText::InputText(InputText&& arg) noexcept : path(std::move(arg.path)), stream(std::move(arg.stream))
    {}

    InputText& InputText::operator=(InputText&& arg) noexcept
    {
        path = std::move(arg.path);
        stream = std::move(arg.stream);
        return *this;
    }

    void InputText::ReadData(std::string& string)
    {
        SanitizeStreamFailure([this, &string]() { stream >> string; }, path);
    }

    void InputText::ReadData(char& character)
    {
        SanitizeStreamFailure([this, &character]() { stream >> character; }, path);
    }

    std::string InputText::ReadLine()
    {
        return SanitizeStreamFailure<std::string>(
            [this]()
            {
                std::string string;
                std::getline(stream, string);
                return string;
            },
            path);
    }

    std::string InputText::ReadLine(char delimiter)
    {
        return SanitizeStreamFailure<std::string>(
            [this, delimiter]()
            {
                std::string string;
                std::getline(stream, string, delimiter);
                return string;
            },
            path);
    }

    std::string InputText::ReadSize(size_t size)
    {
        return SanitizeStreamFailure<std::string>(
            [this]()
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
            },
            path);
    }

    void InputText::Seek(File::Position position)
    {
        SanitizeStreamFailure([this, position]() { stream.seekg(position); }, path);
    }

    Position InputText::Position()
    {
        return SanitizeStreamFailure<File::Position>([this]() { return stream.tellg(); }, path);
    }

    bool InputText::IsAtEnd() const
    {
        return stream.eof();
    }
}