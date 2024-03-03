#pragma once

#include <string>

#include "Scribe.h"

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
        static void Construct(ObjectT* storage, ArchiveT& archive);
    private:
        ScribeBase() = delete;
        ScribeBase(const ScribeBase& arg) = delete;
        ScribeBase& operator=(const ScribeBase& arg) = delete;
    private:
        static void ConstructImplementation(ObjectT* storage, ArchiveT& archive, std::true_type);
        static void ConstructImplementation(ObjectT* storage, ArchiveT& archive, std::false_type);
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object, class Archive>
    void ScribeBase<Object, Archive>::Construct(ObjectT* storage, ArchiveT& archive)
    {
        static_assert(std::is_default_constructible_v<ObjectT>,
            "Construction of this object requires a default constructor. "
            "Define it or shadow Construct for the particular Scribe.");

        return ConstructImplementation(storage, archive,
            std::bool_constant<!std::is_abstract_v<ObjectT> && std::is_default_constructible_v<ObjectT>>{});
    }

    template<class Object, class Archive>
    void ScribeBase<Object, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive, std::true_type)
    {
        new (storage) ObjectT();
        Scribe<Object, Archive>::Scriven(*storage, archive);
    }

    template<class Object, class Archive>
    void ScribeBase<Object, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive, std::false_type)
    {
        return nullptr;
    }
}