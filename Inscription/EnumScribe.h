#pragma once

#include "ScribeBase.h"

#include "ObjectTrackingContext.h"

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
        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;

        using BaseT::DoBasicConstruction;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        static_assert( std::is_enum_v<ObjectT>,
            "The Object given to an EnumScribe was not an enum.");
    };

    template<class Object, class Archive>
    void EnumScribe<Object, Archive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        ObjectTrackingContext trackingContext(ObjectTrackingContext::Inactive, archive);
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

    template<class Object, class Archive>
    void EnumScribe<Object, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}