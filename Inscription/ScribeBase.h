#pragma once

#include <string>

#include "Scribe.h"

#include "BinaryArchive.h"
#include "ObjectTrackingTraits.h"

#include "VerifyNonConst.h"

namespace Inscription
{
    template<class Object, class Archive>
    class ScribeBase
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;
    public:
        virtual void Scriven(ObjectT& object, ArchiveT& archive);
    protected:
        ScribeBase() = default;
        ScribeBase(const ScribeBase& arg) = default;

        virtual void ScrivenImplementation(ObjectT& object, ArchiveT& archive) = 0;
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object, class Archive>
    void ScribeBase<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }

    template<class Object>
    class ScribeBase<Object, BinaryArchive>
    {
    public:
        using ObjectT = Object;
        using ArchiveT = BinaryArchive;
    public:
        virtual void Scriven(ObjectT& object, ArchiveT& archive);
        virtual void Construct(ObjectT* storage, ArchiveT& archive);
    protected:
        ScribeBase() = default;
        ScribeBase(const ScribeBase& arg) = default;

        virtual void ScrivenImplementation(ObjectT& object, ArchiveT& archive) = 0;
        virtual void ConstructImplementation(ObjectT* storage, ArchiveT& archive) = 0;

        void DoBasicConstruction(ObjectT* storage, ArchiveT& archive);
    private:
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive, std::true_type);
        void BasicConstructionImplementation(ObjectT* storage, ArchiveT& archive, std::false_type);
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        ConstructImplementation(storage, archive);
    }

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::DoBasicConstruction(ObjectT* storage, ArchiveT& archive)
    {
        BasicConstructionImplementation(storage, archive,
            std::bool_constant<!std::is_abstract_v<ObjectT> && std::is_default_constructible_v<ObjectT>>{});
    }

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::BasicConstructionImplementation(
        ObjectT* storage, ArchiveT& archive, std::true_type)
    {
        static_assert(std::is_default_constructible_v<ObjectT>,
            "Basic construction of this object requires a default constructor. "
            "Define it or override ConstructImplementation for this Scribe.");
        new (storage) ObjectT();
        Scriven(*storage, archive);
    }

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::BasicConstructionImplementation(
        ObjectT* storage, ArchiveT& archive, std::false_type)
    {}
}