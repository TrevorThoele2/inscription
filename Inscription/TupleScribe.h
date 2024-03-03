#pragma once

#include <tuple>

#include "ObjectScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class... Args>
    class Scribe<std::tuple<Args...>, BinaryArchive> : public ObjectScribe<std::tuple<Args...>, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<std::tuple<Args...>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        template<unsigned int I>
        class UnpackTuple
        {
        public:
            static void ScrivenGroup(ObjectT& object, ArchiveT& archive)
            {
                ScrivenSingle(std::get<I - 1>(object), archive);
                UnpackTuple<I - 1>::ScrivenGroup(object, archive);
            }
        private:
            template<class T>
            static void ScrivenSingle(T& single, ArchiveT& archive)
            {
                archive(single);
            }
        };

        template<>
        class UnpackTuple<0>
        {
        public:
            static void ScrivenGroup(ObjectT& object, ArchiveT& archive)
            {}
        };
    };

    template<class... Args>
    void Scribe<std::tuple<Args...>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        UnpackTuple<sizeof...(Args)>::ScrivenGroup(object, archive);
    }
}