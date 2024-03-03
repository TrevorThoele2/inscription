#pragma once

#include "OutputBinaryArchive.h"
#include "OutputBinaryFormat.h"
#include "InputBinaryArchive.h"
#include "InputBinaryFormat.h"

namespace Inscription
{
    struct Binary
    {
        using TypeRegistrationContext = TypeRegistrationContext<Format::Binary>;

        template<class T>
        static void ToFile(
            T& object,
            const File::Path& path,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            File::OutputBinary file(path);
            Archive::OutputBinary archive(file);
            Format::OutputBinary format(archive, typeRegistrationContext);
            format(object);
        }

        template<class T>
        [[nodiscard]] static Buffer ToBuffer(
            T& object,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            Buffer buffer;
            Archive::OutputBinary archive(buffer);
            Format::OutputBinary format(archive, typeRegistrationContext);
            format(object);
            return buffer;
        }

        template<class T>
        static void FromFile(
            T& object,
            const File::Path& path,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            File::InputBinary file(path);
            Archive::InputBinary archive(file);
            Format::InputBinary format(archive, typeRegistrationContext);
            format(object);
        }

        template<class T>
        static void FromBuffer(
            T& object, 
            const Buffer& buffer,
            const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext())
        {
            auto useBuffer = buffer;
            Archive::InputBinary archive(useBuffer);
            Format::InputBinary format(archive, typeRegistrationContext);
            format(object);
        }
    };
}