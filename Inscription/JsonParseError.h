#pragma once

#include "Exception.h"

namespace Inscription
{
    class JsonParseError final : public Exception
    {
    public:
        JsonParseError();
        JsonParseError(const std::string& message);
    };
}