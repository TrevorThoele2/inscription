#include "InputTextFile.h"

namespace Inscription
{
    InputTextFile::InputTextFile(const FilePath& path) : SimpleFile(path, std::ios::in)
    {}

    InputTextFile::InputTextFile(InputTextFile&& arg) : SimpleFile(std::move(arg))
    {}

    InputTextFile& InputTextFile::operator=(InputTextFile&& arg)
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
        std::string string;
        char* buffer = nullptr;
        stream.get(buffer, size + 1);
        string = buffer;
        return string;
    }
}