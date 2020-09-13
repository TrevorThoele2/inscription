#pragma once

#include "TrackingScribeCategory.h"

#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

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
        static void Scriven(ObjectT& object, BinaryArchive& archive, ScribeT& scribe);
        static void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive, ScribeT& scribe);
    private:
        static_assert(std::is_class_v<ObjectT>,
            "The Object given to a CompositeScribeCategory is not a class/struct.");
    };

    template<class Object>
    void CompositeScribeCategory<Object>::Scriven(
        ObjectT& object, BinaryArchive& archive, ScribeT& scribe)
    {
        TrackingScribeCategory<Object>::Scriven(object, archive, scribe);
    }

    template<class Object>
    void CompositeScribeCategory<Object>::Scriven(
        const std::string& name, ObjectT& object, JsonArchive& archive, ScribeT& scribe)
    {
        if (archive.IsOutput())
        {
            auto output = archive.AsOutput();
            output->StartObject(name);
            scribe.Scriven(object, archive);
            output->EndObject();
        }
        else
        {
            auto input = archive.AsInput();
            input->StartObject(name);
            scribe.Scriven(object, archive);
            input->EndObject();
        }
    }
}