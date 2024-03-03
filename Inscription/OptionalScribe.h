#pragma once

#include <optional>
#include "CompositeScribe.h"
#include "NumericScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T>
    class Scribe<std::optional<T>, BinaryArchive> final :
        public CompositeScribe<std::optional<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<std::optional<T>, BinaryArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(ObjectT& object, ArchiveT& archive);
        void LoadImplementation(ObjectT& object, ArchiveT& archive);
    };

    template<class T>
    void Scribe<std::optional<T>, BinaryArchive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(object, archive);
        else
            LoadImplementation(object, archive);
    }

    template<class T>
    void Scribe<std::optional<T>, BinaryArchive>::SaveImplementation(ObjectT& object, ArchiveT& archive)
    {
        auto hasValue = object.has_value();
        archive(hasValue);

        if (hasValue)
            archive(*object);
    }

    template<class T>
    void Scribe<std::optional<T>, BinaryArchive>::LoadImplementation(ObjectT& object, ArchiveT& archive)
    {
        bool hasValue;
        archive(hasValue);

        if (hasValue)
        {
            ScopeConstructor<T> constructor(archive);
            object = { std::move(constructor.GetMove()) };
        }
        else
            object = {};
    }
}