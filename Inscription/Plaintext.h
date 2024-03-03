#pragma once

#include "OutputTextArchive.h"
#include "OutputPlaintextFormat.h"
#include "InputTextArchive.h"
#include "InputPlaintextFormat.h"

namespace Inscription
{
    struct Plaintext
    {
        using TypeRegistrationContext = TypeRegistrationContext<Format::Plaintext>;

        static void ToFile(const std::string& string, const File::Path& path, bool append = false);

        [[nodiscard]] static std::string FromFile(const File::Path& path);
    };
}