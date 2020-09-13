#pragma once

#include "ScribeBase.h"
#include "BinaryArchive.h"
#include "JsonArchive.h"

namespace Inscription
{
    template<class Object, class Archive>
    class NullScribe;

    template<class Object>
    class NullScribe<Object, BinaryArchive> : public ScribeBase<Object, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        void Scriven(ObjectT& object, ArchiveT& archive) override final;
    protected:
        NullScribe() = default;
        NullScribe(const NullScribe& arg) = default;

        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override final;
    };

    template<class Object>
    void NullScribe<Object, BinaryArchive>::Scriven(ObjectT& object, ArchiveT& archive)
    {}

    template<class Object>
    void NullScribe<Object, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {}

    template<class Object>
    class NullScribe<Object, JsonArchive> : public ScribeBase<Object, JsonArchive>
    {
    private:
        using BaseT = ScribeBase<Object, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        void Scriven(const std::string& name, ObjectT& object, ArchiveT& archive) override final;
    protected:
        NullScribe() = default;
        NullScribe(const NullScribe& arg) = default;

        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override final;
    };

    template<class Object>
    void NullScribe<Object, JsonArchive>::Scriven(const std::string& name, ObjectT& object, ArchiveT& archive)
    {}

    template<class Object>
    void NullScribe<Object, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {}
}