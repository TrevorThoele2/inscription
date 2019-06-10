#include "TextArchive.h"

namespace Inscription
{
    TextArchive::~TextArchive()
    {}

    TextArchive& TextArchive::LoadLine(std::string& arg)
    {
        ReadLineImpl(arg);
        return *this;
    }

    TextArchive& TextArchive::LoadLine(std::string& arg, char delimiter)
    {
        ReadLineImpl(arg, delimiter);
        return *this;
    }

    TextArchive& TextArchive::LoadSize(std::string& arg, size_t size)
    {
        ReadSizeImpl(arg, size);
        return *this;
    }

    bool TextArchive::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool TextArchive::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    TextArchive::TextArchive(Direction direction) : direction(direction)
    {}
}