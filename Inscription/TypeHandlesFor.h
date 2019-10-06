#pragma once

#include <vector>
#include <algorithm>

#include "TypeHandle.h"
#include "ScribeHasRepresentedTypeHandles.h"
#include "RepresentedTypeHandlesAlreadyRegistered.h"

namespace Inscription
{
    template<class T, class Archive>
    TypeHandle PrincipleTypeHandleFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        return RegisteredScribe::PrincipleTypeHandle(archive);
    }

    template<class T, class Archive, std::enable_if_t<scribe_has_represented_type_handles_v<T, Archive>, int> = 0>
    std::vector<TypeHandle> RepresentedTypeHandlesFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        auto principleTypeHandle = RegisteredScribe::PrincipleTypeHandle(archive);
        auto representedTypeHandles = RegisteredScribe::RepresentedTypeHandles(archive);

        {
            std::vector<TypeHandle> duplicateTypeHandles;
            for (auto& currentRepresented : representedTypeHandles)
            {
                auto count = std::count(
                    representedTypeHandles.begin(),
                    representedTypeHandles.end(),
                    currentRepresented);
                if (count > 1)
                    duplicateTypeHandles.push_back(currentRepresented);
            }

            if (!duplicateTypeHandles.empty())
                throw RepresentedTypeHandlesAlreadyRegistered(duplicateTypeHandles);
        }

        auto isPrincipleIn = [principleTypeHandle](const TypeHandle& typeHandle)
        {
            return typeHandle == principleTypeHandle;
        };

        if (!std::any_of(representedTypeHandles.begin(), representedTypeHandles.end(), isPrincipleIn))
            representedTypeHandles.push_back(principleTypeHandle);

        return representedTypeHandles;
    }

    template<class T, class Archive, std::enable_if_t<!scribe_has_represented_type_handles_v<T, Archive>, int> = 0>
    static std::vector<TypeHandle> RepresentedTypeHandlesFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        return std::vector<TypeHandle> { RegisteredScribe::PrincipleTypeHandle(archive) };
    }
}