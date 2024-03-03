#pragma once

#include <optional>
#include "ObjectScribe.h"
#include "NumericScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T>
    class Scribe<std::optional<T>, BinaryArchive> final :
        public ObjectScribe<std::optional<T>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<std::optional<T>, BinaryArchive>;
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

    class JsonArchive;

    template<class T>
    class Scribe<std::optional<T>, JsonArchive> final :
        public ObjectScribe<std::optional<T>, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<std::optional<T>, JsonArchive>;
    public:
        using typename BaseT::ObjectT;
        using typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override;
    private:
        void SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive);
        void LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive);
    };

    template<class T>
    void Scribe<std::optional<T>, JsonArchive>::ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            SaveImplementation(name, object, archive);
        else
            LoadImplementation(name, object, archive);
    }

    template<class T>
    void Scribe<std::optional<T>, JsonArchive>::SaveImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (!object.has_value())
        {
            auto output = archive.AsOutput();
            output->WriteValue(name, "null");
        }
        else
            archive(name, *object);
    }

    template<class T>
    void Scribe<std::optional<T>, JsonArchive>::LoadImplementation(const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        std::string readValue;
        archive.AsInput()->ReadValue(name, readValue);

        if (readValue == "null")
            object = {};
        else
        {
            ScopeConstructor<T> constructor(name, archive);
            object = { std::move(constructor.GetMove()) };
        }
    }
}