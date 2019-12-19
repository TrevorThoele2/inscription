#pragma once

#include "Scribe.h"
#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class Scribe<Object*, Archive> : public ScribeBase<Object*, Archive>
    {
    private:
        using BaseT = ScribeBase<Object*, Archive>;
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
}