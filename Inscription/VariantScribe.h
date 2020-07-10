#pragma once

#include <variant>

#include "CompositeScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class... Args>
    class Scribe<std::variant<Args...>, BinaryArchive> : public CompositeScribe<std::variant<Args...>, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<std::variant<Args...>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        template<unsigned int I>
        class UnpackVariant
        {
        public:
            static void AttemptSaveValue(ObjectT& object, ArchiveT& archive)
            {
                if (object.index() == I - 1)
                {
                    ScrivenSingle(std::get<I - 1>(object), archive);
                    return;
                }
                UnpackVariant<I - 1>::AttemptSaveValue(object, archive);
            }

            static void AttemptLoadValue(std::size_t index, ObjectT& object, ArchiveT& archive)
            {
                if (index == I - 1)
                {
                    std::variant_alternative_t<I - 1, ObjectT> value;
                    ScrivenSingle(value, archive);
                    object = value;
                    return;
                }
                UnpackVariant<I - 1>::AttemptLoadValue(index, object, archive);
            }
        private:
            template<class T>
            static void ScrivenSingle(T& single, ArchiveT& archive)
            {
                archive(single);
            }
        };

        template<>
        class UnpackVariant<0>
        {
        public:
            static void AttemptSaveValue(ObjectT& object, ArchiveT& archive)
            {}

            static void AttemptLoadValue(std::size_t index, ObjectT& object, ArchiveT& archive)
            {}
        };
    };

    template<class... Args>
    void Scribe<std::variant<Args...>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto index = object.index();
            archive(index);
            UnpackVariant<sizeof...(Args)>::AttemptSaveValue(object, archive);
        }
        else
        {
            std::size_t index;
            archive(index);
            UnpackVariant<sizeof...(Args)>::AttemptLoadValue(index, object, archive);
        }
    }
}