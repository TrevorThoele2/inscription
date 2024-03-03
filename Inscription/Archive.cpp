#include "Archive.h"

namespace Inscription
{
    Archive::~Archive() = default;

    Archive::Archive(Archive&& arg) noexcept : userContexts(std::move(arg.userContexts))
    {}

    Archive& Archive::operator=(Archive&& arg) noexcept
    {
        userContexts = std::move(arg.userContexts);
        return *this;
    }
}