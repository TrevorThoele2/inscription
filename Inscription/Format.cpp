#include "Format.h"

namespace Inscription::Format
{
    Format::~Format() = default;

    Format::Format(Format&& arg) noexcept : userContexts(std::move(arg.userContexts))
    {}

    Format& Format::operator=(Format&& arg) noexcept
    {
        userContexts = std::move(arg.userContexts);
        return *this;
    }
}