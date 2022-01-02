#pragma once

#include <string>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/StringScribe.h>
#include <Inscription/NumericScribe.h>

class NamedClass
{
public:
    std::string name;
    int value = 0;

    NamedClass() = default;
    NamedClass(const std::string& name, int value) : name(name), value(value)
    {}

    bool operator==(const NamedClass& arg) const
    {
        return value == arg.value;
    }
};

namespace Inscription
{
    template<>
    class Scribe<NamedClass> final
    {
    public:
        using ObjectT = NamedClass;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            format(object.name, object.value);
        }
    };

    template<class Format>
    struct ScribeTraits<NamedClass, Format> final
    {
        using Category = CompositeScribeCategory<NamedClass>;
    };
}