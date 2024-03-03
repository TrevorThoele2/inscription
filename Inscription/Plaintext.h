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

        template<class T>
        static void ToFile(
            T& object,
            const File::Path& path,
            bool append = false,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            File::OutputText file(path, append);
            Archive::OutputText archive(file);
            Format::OutputPlaintext format(object, archive, typeRegistrationContext);
        }

        template<class T>
        [[nodiscard]] static std::string ToString(
            T& object,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            std::string string;
            Archive::OutputText archive(string);
            Format::OutputPlaintext format(object, archive, typeRegistrationContext);
            return string;
        }

        template<class T>
        static void FromFile(
            T& object,
            const File::Path& path,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            File::InputText file(path);
            Archive::InputText archive(file);
            Format::InputPlaintext format(object, archive, typeRegistrationContext);
        }

        template<class T>
        static void FromString(
            T& object,
            const std::string& string,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            auto useString = string;
            Archive::InputText archive(useString);
            Format::InputPlaintext format(object, archive, typeRegistrationContext);
        }
    };
}