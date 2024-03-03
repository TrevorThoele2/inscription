#include "InputTextFile.h"

namespace Inscription
{
    InputTextFile::InputTextFile(const Path& path) : SimpleFile(path, std::ios::in)
    {}

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