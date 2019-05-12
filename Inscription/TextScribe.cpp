#include "TextScribe.h"

namespace Inscription
{
    TextScribe::~TextScribe()
    {}

    TextScribe& TextScribe::LoadLine(std::string& arg)
    {
        ReadLineImpl(arg);
        return *this;
    }

    TextScribe& TextScribe::LoadLine(std::string& arg, char delimiter)
    {
        ReadLineImpl(arg, delimiter);
        return *this;
    }

    TextScribe& TextScribe::LoadSize(std::string& arg, size_t size)
    {
        ReadSizeImpl(arg, size);
        return *this;
    }

    bool TextScribe::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool TextScribe::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    TextScribe::TextScribe(Direction direction) : direction(direction)
    {}
}