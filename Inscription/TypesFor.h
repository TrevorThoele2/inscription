#pragma once

#include <vector>
#include <algorithm>

#include "Type.h"
#include "ScribeHasOutputType.h"
#include "ScribeHasInputTypes.h"
#include "InputTypesAlreadyRegistered.h"

namespace Inscription
{
    template<class T, class Format, std::enable_if_t<scribe_has_output_type_v<T, Format>, int> = 0>
    Type OutputTypeFor(Format& format)
    {
        using RegisteredScribe = Scribe<T>;
        return RegisteredScribe::OutputType(format);
    }

    template<class T, class Format, std::enable_if_t<!scribe_has_output_type_v<T, Format>, int> = 0>
    Type OutputTypeFor(Format& format)
    {
        using RegisteredScribeCategory = typename ScribeTraits<T, Format>::Category;
        return RegisteredScribeCategory::OutputType(format);
    }

    template<class T, class Format, std::enable_if_t<scribe_has_input_types_v<T, Format>, int> = 0>
    std::vector<Type> InputTypesFor(Format& format)
    {
        using RegisteredScribe = Scribe<T>;
        auto outputType = OutputTypeFor<T>(format);
        auto inputTypes = RegisteredScribe::InputTypes(format);

        {
            std::vector<Type> duplicateTypes;
            for (auto& inputType : inputTypes)
            {
                auto count = std::count(
                    inputTypes.begin(),
                    inputTypes.end(),
                    inputType);
                if (count > 1)
                    duplicateTypes.push_back(inputType);
            }

            if (!duplicateTypes.empty())
                throw InputTypesAlreadyRegistered(duplicateTypes);
        }

        auto isOutputTypeIn = [outputType](const Type& type)
        {
            return type == outputType;
        };

        if (!std::any_of(inputTypes.begin(), inputTypes.end(), isOutputTypeIn))
            inputTypes.push_back(outputType);

        return inputTypes;
    }

    template<class T, class Format, std::enable_if_t<!scribe_has_input_types_v<T, Format>, int> = 0>
    static std::vector<Type> InputTypesFor(Format& format)
    {
        using RegisteredScribe = Scribe<T>;
        return std::vector<Type> { OutputTypeFor<T>(format) };
    }
}