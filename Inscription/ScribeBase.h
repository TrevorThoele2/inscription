#pragma once

#include "BinaryArchive.h"
#include "JsonArchive.h"
#include "TextArchive.h"

#include "VerifyNonConst.h"

namespace Inscription
{
    template<class Object, class Archive>
    class ScribeBase;

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

    template<class Object>
    class ScribeBase<Object, JsonArchive>
    {
    public:
        using ObjectT = Object;
        using ArchiveT = JsonArchive;
    public:
        virtual ~ScribeBase() = default;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive);
        virtual void Scriven(const std::string& name, ObjectT& object, ArchiveT& archive);
    protected:
        ScribeBase() = default;
        ScribeBase(const ScribeBase& arg) = default;

        virtual void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) = 0;
    private:
        VERIFY_NON_CONST(ObjectT);
    };

    template<class Object>
    void ScribeBase<Object, JsonArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        Scriven("", object, archive);
    }

    template<class Object>
    void ScribeBase<Object, JsonArchive>::Scriven(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(name, object, archive);
    }

    template<class Object>
    class ScribeBase<Object, TextArchive>
    {
    public:
        using ObjectT = Object;
        using ArchiveT = TextArchive;
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

    template<class Object>
    ScribeBase<Object, TextArchive>::~ScribeBase() = default;

    template<class Object>
    void ScribeBase<Object, TextArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {
        ScrivenImplementation(object, archive);
    }
}