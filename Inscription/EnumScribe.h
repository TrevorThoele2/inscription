#pragma once

#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class EnumScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    public:
        using BaseT::Construct;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        EnumScribe() = delete;
        EnumScribe(const EnumScribe& arg) = delete;
        EnumScribe& operator=(const EnumScribe& arg) = delete;
    private:
        static_assert( std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribe was not an enum.");
    };

    template<class Object, class Archive>
    void EnumScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
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
}