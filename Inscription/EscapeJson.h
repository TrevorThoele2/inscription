#pragma once

#include <string>

namespace Inscription::Format
{
    [[nodiscard]] std::string EscapeJson(const std::string& input);
}