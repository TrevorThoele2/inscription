#include "JsonParseError.h"

namespace Inscription
{
    JsonParseError::JsonParseError() : JsonParseError("There was an error parsing JSON.")
    {}

    JsonParseError::JsonParseError(const std::string& message) : Exception(message)
    {}
}