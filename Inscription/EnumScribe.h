#pragma once

#include "ScribeBase.h"

#include "ObjectTrackingContext.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    template<class Object, class Archive>
    class EnumScribe;

    template<class Object>
    class EnumScribe<Object, BinaryArchive> : public ScribeBase<Object, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        static_assert( std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribe was not an enum.");
    };

    template<class Object>
    void EnumScribe<Object, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
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
    class EnumScribe<Object, JsonArchive> : public ScribeBase<Object, JsonArchive>
    {
    private:
        using BaseT = ScribeBase<Object, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        static_assert(std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribe was not an enum.");
    };

    template<class Object>
    void EnumScribe<Object, JsonArchive>::ScrivenImplementation(
        const std::string& name, ObjectT& object, ArchiveT& archive)
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