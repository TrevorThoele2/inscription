#include "EscapeJson.h"

namespace Inscription::Format
{
    std::string EscapeJson(const std::string& input)
    {
        std::string output;
        for (auto& character : input)
        {
            switch (character)
            {
            case '\"':
            case '\\':
                output.push_back('\\');
                output.push_back(character);
                break;
            default:
                output.push_back(character);
            }
        }
        return output;
    }
}