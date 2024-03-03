#pragma once

#include <string>

#include "BinaryArchive.h"

#include "VerifyNonConst.h"

#include <Chroma/IsBracesConstructible.h>

namespace Inscription
{
    template<class Object, class Archive>
    class ScribeBase
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;
    public:
        virtual ~ScribeBase() = 0;
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
    ScribeBase<Object, Archive>::~ScribeBase()
    {}

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
        virtual ~ScribeBase() = default;
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
        template<
            class U,
            std::enable_if_t<!std::is_abstract_v<U> && ::Chroma::is_braces_default_constructible_v<U>, int> = 0>
        void BasicConstructionImplementation(U* storage, ArchiveT& archive);
        template<
            class U,
            std::enable_if_t<!std::is_abstract_v<U> && !::Chroma::is_braces_default_constructible_v<U>, int> = 0>
        void BasicConstructionImplementation(U* storage, ArchiveT& archive);
        template<
            class U,
            std::enable_if_t<std::is_abstract_v<U>, int> = 0>
            void BasicConstructionImplementation(U* storage, ArchiveT& archive);
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
        BasicConstructionImplementation(storage, archive);
    }

    template<class Object>
    template<
        class U,
        std::enable_if_t<!std::is_abstract_v<U> && ::Chroma::is_braces_default_constructible_v<U>, int>>
    void ScribeBase<Object, BinaryArchive>::BasicConstructionImplementation(U* storage, ArchiveT& archive)
    {
        new (storage) ObjectT{};
        Scriven(*storage, archive);
    }

    template<class Object>
    template<
        class U,
        std::enable_if_t<!std::is_abstract_v<U> && !::Chroma::is_braces_default_constructible_v<U>, int>>
    void ScribeBase<Object, BinaryArchive>::BasicConstructionImplementation(U* storage, ArchiveT& archive)
    {
        static_assert(false,
            "Basic construction of this object requires a default constructor. "
            "Define it or override ConstructImplementation for this Scribe.");
    }

    template<class Object>
    template<
        class U,
        std::enable_if_t<std::is_abstract_v<U>, int>>
    void ScribeBase<Object, BinaryArchive>::BasicConstructionImplementation(U* storage, ArchiveT& archive)
    {}
}