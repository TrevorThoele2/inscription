#pragma once

#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class NullScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
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

    template<class Object, class Archive>
    void NullScribe<Object, Archive>::Scriven(ObjectT& object, ArchiveT& archive)
    {}

    template<class Object, class Archive>
    void NullScribe<Object, Archive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {}
}