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
        static void Construct(ObjectT*& storage, ArchiveT& archive);
        static ObjectT* Construct(ArchiveT& archive);
    private:
        ScribeBase() = delete;
        ScribeBase(const ScribeBase& arg) = delete;
        ScribeBase& operator=(const ScribeBase& arg) = delete;
    private:
        static ObjectT* ConstructImplementation(ArchiveT& archive, std::true_type);
        static ObjectT* ConstructImplementation(ArchiveT& archive, std::false_type);
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object, class Archive>
    void ScribeBase<Object, Archive>::Construct(ObjectT*& storage, ArchiveT& archive)
    {
        storage = Construct(archive);
    }

    template<class Object, class Archive>
    typename ScribeBase<Object, Archive>::ObjectT* ScribeBase<Object, Archive>::Construct(ArchiveT& archive)
    {
        static_assert(std::is_default_constructible_v<ObjectT>,
            "Construction of this object requires a default constructor. Define it or shadow Construct for the particular Scribe.");
        return ConstructImplementation(archive,
            std::bool_constant<!std::is_abstract_v<ObjectT> && std::is_default_constructible_v<ObjectT>>{});
    }

    template<class Object, class Archive>
    typename ScribeBase<Object, Archive>::ObjectT* ScribeBase<Object, Archive>::ConstructImplementation(ArchiveT& archive, std::true_type)
    {
        auto created = new ObjectT();
        Scribe<Object, Archive>::Scriven(*created, archive);
        return created;
    }

    template<class Object, class Archive>
    typename ScribeBase<Object, Archive>::ObjectT* ScribeBase<Object, Archive>::ConstructImplementation(ArchiveT& archive, std::false_type)
    {
        return nullptr;
    }
}