#pragma once

#include <variant>

#include "TrackingScribeCategory.h"

#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    class BinaryArchive;

    namespace detail
    {
        template<unsigned int I>
        class UnpackVariant
        {
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>& object, BinaryArchive& archive)
            {
                if (object.index() == I - 1)
                {
                    auto& value = std::get<I - 1>(object);
                    archive(value);
                    return;
                }
                UnpackVariant<I - 1>::AttemptSaveValue(object, archive);
            }

            template<class... Args>
            static void AttemptLoadValue(std::size_t index, std::variant<Args...>& object, BinaryArchive& archive)
            {
                if (index == I - 1)
                {
                    std::variant_alternative_t<I - 1, std::variant<Args...>> value;
                    archive(value);
                    object = value;
                    return;
                }
                UnpackVariant<I - 1>::AttemptLoadValue(index, object, archive);
            }
        public:

            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>& object, JsonArchive& archive)
            {
                if (object.index() == I - 1)
                {
                    auto& value = std::get<I - 1>(object);
                    archive("value", value);
                    return;
                }
                UnpackVariant<I - 1>::AttemptSaveValue(object, archive);
            }

            template<class... Args>
            static void AttemptLoadValue(std::size_t index, std::variant<Args...>& object, JsonArchive& archive)
            {
                if (index == I - 1)
                {
                    std::variant_alternative_t<I - 1, std::variant<Args...>> value;
                    archive("value", value);
                    object = value;
                    return;
                }
                UnpackVariant<I - 1>::AttemptLoadValue(index, object, archive);
            }
        };

        template<>
        class UnpackVariant<0>
        {
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>&, BinaryArchive&)
            {}

            template<class... Args>
            static void AttemptLoadValue(std::size_t, std::variant<Args...>&, BinaryArchive&)
            {}
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>&, JsonArchive&)
            {}

            template<class... Args>
            static void AttemptLoadValue(std::size_t, std::variant<Args...>&, JsonArchive&)
            {}
        };
    }

    template<class... Args>
    class Scribe<std::variant<Args...>> final
    {
    public:
        using ObjectT = std::variant<Args...>;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive);
        void Scriven(const std::string& name, ObjectT& object, JsonArchive& archive);
    };

    template<class... Args>
    void Scribe<std::variant<Args...>>::Scriven(ObjectT& object, BinaryArchive& archive)
    {
        if (archive.IsOutput())
        {
            auto index = object.index();
            archive(index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptSaveValue(object, archive);
        }
        else
        {
            std::size_t index;
            archive(index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptLoadValue(index, object, archive);
        }
    }

    template<class... Args>
    void Scribe<std::variant<Args...>>::Scriven(const std::string& name, ObjectT& object, JsonArchive& archive)
    {
        if (archive.IsOutput())
        {
            auto output = archive.AsOutput();
            output->StartObject(name);

            auto index = object.index();
            archive("index", index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptSaveValue(object, archive);

            output->EndObject();
        }
        else
        {
            auto input = archive.AsInput();
            input->StartObject(name);

            std::size_t index;
            archive("index", index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptLoadValue(index, object, archive);

            input->EndObject();
        }
    }

    template<class... Args, class Archive>
    struct ScribeTraits<std::variant<Args...>, Archive>
    {
        using Category = TrackingScribeCategory<std::variant<Args...>>;
    };
}