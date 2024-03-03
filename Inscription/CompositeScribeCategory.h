#pragma once

#include "TrackingScribeCategory.h"

#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    template<class Object>
    class CompositeScribeCategory final
    {
    public:
        using ObjectT = Object;
    public:
        static constexpr bool requiresScribe = true;
        using ScribeT = Scribe<Object>;
    public:
        static void Scriven(ObjectT& object, Format::Binary& format, ScribeT& scribe);
        static void Scriven(const std::string& name, ObjectT& object, Format::Json& format, ScribeT& scribe);
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribeCategory is not a class/struct.");
    };

    template<class Object>
    void CompositeScribeCategory<Object>::Scriven(
        ObjectT& object, Format::Binary& format, ScribeT& scribe)
    {
        TrackingScribeCategory<Object>::Scriven(object, format, scribe);
    }

    template<class Object>
    void CompositeScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT& object, Format::Json& format, ScribeT& scribe)
    {
        if (format.IsOutput())
        {
            const auto output = format.AsOutput();
            output->StartObject(name);
            scribe.Scriven(object, format);
            output->EndObject();
        }
        else
        {
            const auto input = format.AsInput();
            input->StartObject(name);
            scribe.Scriven(object, format);
            input->EndObject();
        }
    }
}