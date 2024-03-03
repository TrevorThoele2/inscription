#pragma once

#include "ObjectTrackingContext.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"
#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    template<class Object>
    class EnumScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = false;
        using ScribeT = Scribe<Object>;
    public:
        static void Scriven(ObjectT& object, Archive::Binary& archive);
        static void Scriven(const std::string& name, ObjectT& object, Archive::Binary& archive);
        static void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    private:
        static_assert(std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribeCategory is not an enum.");
    };

    template<class Object>
    void EnumScribeCategory<Object>::Scriven(
        ObjectT& object, Archive::Binary& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
        {
            auto castedObject = static_cast<std::underlying_type_t<Object>>(object);
            archive.AsOutput()->Write(castedObject);
        }
        else
        {
            std::underlying_type_t<Object> loaded;
            archive.AsInput()->Read(loaded);
            object = static_cast<Object>(loaded);
        }
    }

    template<class Object>
    void EnumScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(archive.types);
        if (archive.IsOutput())
        {
            auto castedObject = Chroma::ToString(static_cast<std::underlying_type_t<Object>>(object));
            archive.AsOutput()->WriteValue(name, castedObject);
        }
        else
        {
            std::string loaded;
            archive.AsInput()->TakeValue(name, loaded);
            object = static_cast<Object>(Chroma::FromString<std::underlying_type_t<Object>>(loaded));
        }
    }
}