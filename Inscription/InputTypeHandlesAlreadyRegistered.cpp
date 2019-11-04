#include "InputTypeHandlesAlreadyRegistered.h"

namespace Inscription
{
    InputTypeHandlesAlreadyRegistered::InputTypeHandlesAlreadyRegistered() :
        Exception(
            DefaultString())
    {}

    InputTypeHandlesAlreadyRegistered::InputTypeHandlesAlreadyRegistered(
        const std::vector<TypeHandle>& typeHandles)
        :
        Exception(
            DefaultString() +
            " TypeHandles: " + Join(typeHandles) + ".")
    {}

    std::string InputTypeHandlesAlreadyRegistered::DefaultString()
    {
        return std::string(
            "Input type handles were already registered in the PolymorphicManager.");
    }

    std::string InputTypeHandlesAlreadyRegistered::Join(const std::vector<TypeHandle>& typeHandles)
    {
        std::string built;
        if (typeHandles.empty())
            return built;

        for(size_t i = 0;; ++i)
        {
            built += typeHandles[0];

            if (i < typeHandles.size() - 1)
                built += ", ";
            else if (i == typeHandles.size() - 1)
                break;
        }
        return built;
    }
}