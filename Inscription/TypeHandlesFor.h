#pragma once

#include <vector>
#include <algorithm>

#include "TypeHandle.h"
#include "ScribeHasInputTypeHandles.h"
#include "InputTypeHandlesAlreadyRegistered.h"

namespace Inscription
{
    template<class T, class Archive>
    TypeHandle OutputTypeHandleFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        return RegisteredScribe::OutputTypeHandle(archive);
    }

    template<class T, class Archive, std::enable_if_t<scribe_has_input_type_handles_v<T, Archive>, int> = 0>
    std::vector<TypeHandle> InputTypeHandlesFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        auto outputTypeHandle = RegisteredScribe::OutputTypeHandle(archive);
        auto inputTypeHandles = RegisteredScribe::InputTypeHandles(archive);

        {
            std::vector<TypeHandle> duplicateTypeHandles;
            for (auto& inputTypeHandle : inputTypeHandles)
            {
                auto count = std::count(
                    inputTypeHandles.begin(),
                    inputTypeHandles.end(),
                    inputTypeHandle);
                if (count > 1)
                    duplicateTypeHandles.push_back(inputTypeHandle);
            }

            if (!duplicateTypeHandles.empty())
                throw InputTypeHandlesAlreadyRegistered(duplicateTypeHandles);
        }

        auto isOutputTypeHandleIn = [outputTypeHandle](const TypeHandle& typeHandle)
        {
            return typeHandle == outputTypeHandle;
        };

        if (!std::any_of(inputTypeHandles.begin(), inputTypeHandles.end(), isOutputTypeHandleIn))
            inputTypeHandles.push_back(outputTypeHandle);

        return inputTypeHandles;
    }

    template<class T, class Archive, std::enable_if_t<!scribe_has_input_type_handles_v<T, Archive>, int> = 0>
    static std::vector<TypeHandle> InputTypeHandlesFor(Archive& archive)
    {
        using RegisteredScribe = Scribe<T, Archive>;
        return std::vector<TypeHandle> { RegisteredScribe::OutputTypeHandle(archive) };
    }
}