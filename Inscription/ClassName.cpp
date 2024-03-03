
#include "ClassName.h"
#include "Scribe.h"

namespace Inscription
{
    bool EqualComp(Version first, Version second)
    {
        return first == second;
    }

    bool LesserComp(Version first, Version second)
    {
        return first < second;
    }

    bool GreaterComp(Version first, Version second)
    {
        return first > second;
    }

    bool AnyComp(Version first, Version second)
    {
        return true;
    }

    const ClassNameContainer::Name& ClassNameContainer::GetName(Scribe &scribe) const
    {
        auto foundEntries = entries.find(typeid(scribe));
        if (foundEntries == entries.end())
            return defaultName;

        auto version = scribe.GetVersion();

        // Returns non-null pointer if should use this name
        auto func = [&](Op op, bool(*comp)(Version, Version)) -> const Name*
        {
            auto range = foundEntries->second.equal_range(op);
            for (auto loop = range.first; loop != range.second; ++loop)
            {
                if ((*comp)(loop->second.version, version))
                    return &loop->second.name;
            }

            return nullptr;
        };

        // Op order:
        // Equal
        // Less
        // Greater
        // Any

        const Name *result = nullptr;

        result = func(Op::EQUAL, &EqualComp);
        if (result)
            return *result;

        result = func(Op::LESSER, &LesserComp);
        if (result)
            return *result;

        result = func(Op::GREATER, &GreaterComp);
        if (result)
            return *result;

        result = func(Op::ANY, &AnyComp);
        if (result)
            return *result;

        return defaultName;
    }
}