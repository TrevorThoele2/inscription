#pragma once

#include <optional>
#include "TrackingScribeCategory.h"
#include "NumericScribe.h"

#include "ScopeConstructor.h"

namespace Inscription
{
    template<class T>
    class Scribe<std::optional<T>> final
    {
    public:
        using ObjectT = std::optional<T>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class T>
    void Scribe<std::optional<T>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            auto hasValue = object.has_value();
            format(hasValue);

            if (hasValue)
                format(*object);
        }
        else
        {
            bool hasValue;
            format(hasValue);

            if (hasValue)
            {
                ScopeConstructor<T> constructor(format);
                object = { std::move(constructor.GetMove()) };
            }
            else
                object = {};
        }
    }

    template<class T>
    void Scribe<std::optional<T>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            if (!object.has_value())
            {
                auto output = format.AsOutput();
                output->WriteValue(name, "null");
            }
            else
                format(name, *object);
        }
        else
        {
            const auto inputFormat = format.AsInput();

            const auto hasValue = inputFormat->HasValue(name);

            if (inputFormat->IsNull(name))
            {
                object = {};
                return;
            }

            ScopeConstructor<T> constructor(name, format);
            object = { std::move(constructor.GetMove()) };
        }
    }

    template<class T, class Format>
    struct ScribeTraits<std::optional<T>, Format>
    {
        using Category = TrackingScribeCategory<std::optional<T>>;
    };
}