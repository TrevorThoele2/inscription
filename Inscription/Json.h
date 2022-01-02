#pragma once

#include "OutputTextArchive.h"
#include "OutputJsonFormat.h"
#include "InputTextArchive.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    struct Json
    {
        using TypeRegistrationContext = TypeRegistrationContext<Format::Json>;

        template<class T>
        static void ToFile(
            T& object,
            const File::Path& path,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            File::OutputText file(path, false);
            Archive::OutputText archive(file);
            Format::OutputJson format(object, archive, typeRegistrationContext);
        }

        template<class T>
        [[nodiscard]] static std::string ToString(
            T& object,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            std::string string;
            Archive::OutputText archive(string);
            Format::OutputJson format(object, archive, typeRegistrationContext);
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
            Format::InputJson format(object, archive, typeRegistrationContext);
        }

        template<class T>
        static void FromString(
            T& object,
            const std::string& string,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            auto useString = string;
            Archive::InputText archive(useString);
            Format::InputJson format(object, archive, typeRegistrationContext);
        }
    };
}