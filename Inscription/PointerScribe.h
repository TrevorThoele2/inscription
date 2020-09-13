#pragma once

#include "Scribe.h"
#include "ScribeBase.h"

namespace Inscription
{
    template<class Object>
    class Scribe<Object*, BinaryArchive> : public ScribeBase<Object*, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<Object*, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object);
        }
    };

    template<class Object>
    class Scribe<Object*, JsonArchive> : public ScribeBase<Object*, JsonArchive>
    {
    private:
        using BaseT = ScribeBase<Object*, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override
        {
            archive(name, object);
        }
    };
}