#pragma once

#include <string>

namespace Inscription
{
    class BinaryScribe;

    void Serialize(BinaryScribe& scribe, std::string& obj);
}