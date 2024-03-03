#pragma once

#include <optional>
#include "TrackingScribeCategory.h"
#include "NumericScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    class BinaryArchive;

    template<class T>
    class Scribe<std::optional<T>> final
    {
    public:
        using ObjectT = std::optional<T>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class T>
    void Scribe<std::optional<T>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
        {
            auto hasValue = object.has_value();
            archive(hasValue);

            if (hasValue)
                archive(*object);
        }
        else
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

    template<class T>
    void Scribe<std::optional<T>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
        {
            if (!object.has_value())
            {
                auto output = archive.AsOutput();
                output->WriteValue(name, "null");
            }
            else
                archive(name, *object);
        }
        else
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

    template<class T, class Archive>
    struct ScribeTraits<std::optional<T>, Archive>
    {
        using Category = TrackingScribeCategory<std::optional<T>>;
    };
}