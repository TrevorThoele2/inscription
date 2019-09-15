#include "Archive.h"

namespace Inscription
{
    Archive::~Archive() = default;

    Archive::Archive(Archive&& arg) noexcept
    {}

    Archive& Archive::operator=(Archive&& arg) noexcept
    {
        return *this;
    }
}