#pragma once

#include "ObjectTrackingContext.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"
#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

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
        static void Scriven(ObjectT& object, Format::Binary& format);
        static void Scriven(const std::string& name, ObjectT& object, Format::Binary& format);
        static void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    private:
        static_assert(std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribeCategory is not an enum.");
    };

    template<class Object>
    void EnumScribeCategory<Object>::Scriven(
        ObjectT& object, Format::Binary& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        if (format.IsOutput())
        {
            auto castedObject = static_cast<std::underlying_type_t<Object>>(object);
            format.AsOutput()->Write(castedObject);
        }
        else
        {
            std::underlying_type_t<Object> loaded;
            format.AsInput()->Read(loaded);
            object = static_cast<Object>(loaded);
        }
    }

    template<class Object>
    void EnumScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT& object, Format::Json& format)
    {
        auto trackingContext = ObjectTrackingContext::Inactive(format.types);
        if (format.IsOutput())
        {
            auto castedObject = Chroma::ToString(static_cast<std::underlying_type_t<Object>>(object));
            format.AsOutput()->WriteValue(name, castedObject);
        }
        else
        {
            std::string loaded;
            format.AsInput()->TakeValue(name, loaded);
            object = static_cast<Object>(Chroma::FromString<std::underlying_type_t<Object>>(loaded));
        }
    }
}