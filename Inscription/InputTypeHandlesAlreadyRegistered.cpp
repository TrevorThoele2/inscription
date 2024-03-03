#include "InputTypesAlreadyRegistered.h"

namespace Inscription
{
    InputTypesAlreadyRegistered::InputTypesAlreadyRegistered() :
        Exception(
            DefaultString())
    {}

    InputTypesAlreadyRegistered::InputTypesAlreadyRegistered(
        const std::vector<Type>& types)
        :
        Exception(
            DefaultString() +
            " Types: " + Join(types) + ".")
    {}

    std::string InputTypesAlreadyRegistered::DefaultString()
    {
        return std::string(
            "Input type were already registered in the PolymorphicManager.");
    }

    std::string InputTypesAlreadyRegistered::Join(const std::vector<Type>& types)
    {
        std::string built;
        if (types.empty())
            return built;

        for(size_t i = 0;; ++i)
        {
            built += types[0];

            if (i < types.size() - 1)
                built += ", ";
            else if (i == types.size() - 1)
                break;
        }
        return built;
    }
}