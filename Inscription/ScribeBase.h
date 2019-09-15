#pragma once

#include "BinaryArchive.h"

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
    protected:
        ScribeBase() = default;
        ScribeBase(const ScribeBase& arg) = default;

        virtual void ScrivenImplementation(ObjectT& object, ArchiveT& archive) = 0;
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object>
    void ScribeBase<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }
}