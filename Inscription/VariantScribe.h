#pragma once

#include <variant>

#include "TrackingScribeCategory.h"

#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    namespace Format
    {
        class Binary;
    }

    namespace detail
    {
        template<unsigned int I>
        class UnpackVariant
        {
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>& object, Format::Binary& format)
            {
                if (object.index() == I - 1)
                {
                    auto& value = std::get<I - 1>(object);
                    format(value);
                    return;
                }
                UnpackVariant<I - 1>::AttemptSaveValue(object, format);
            }

            template<class... Args>
            static void AttemptLoadValue(std::size_t index, std::variant<Args...>& object, Format::Binary& format)
            {
                if (index == I - 1)
                {
                    std::variant_alternative_t<I - 1, std::variant<Args...>> value;
                    format(value);
                    object = value;
                    return;
                }
                UnpackVariant<I - 1>::AttemptLoadValue(index, object, format);
            }
        public:

            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>& object, Format::Json& format)
            {
                if (object.index() == I - 1)
                {
                    auto& value = std::get<I - 1>(object);
                    format("value", value);
                    return;
                }
                UnpackVariant<I - 1>::AttemptSaveValue(object, format);
            }

            template<class... Args>
            static void AttemptLoadValue(std::size_t index, std::variant<Args...>& object, Format::Json& format)
            {
                if (index == I - 1)
                {
                    std::variant_alternative_t<I - 1, std::variant<Args...>> value;
                    format("value", value);
                    object = value;
                    return;
                }
                UnpackVariant<I - 1>::AttemptLoadValue(index, object, format);
            }
        };

        template<>
        class UnpackVariant<0>
        {
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>&, Format::Binary&)
            {}

            template<class... Args>
            static void AttemptLoadValue(std::size_t, std::variant<Args...>&, Format::Binary&)
            {}
        public:
            template<class... Args>
            static void AttemptSaveValue(std::variant<Args...>&, Format::Json&)
            {}

            template<class... Args>
            static void AttemptLoadValue(std::size_t, std::variant<Args...>&, Format::Json&)
            {}
        };
    }

    template<class... Args>
    class Scribe<std::variant<Args...>> final
    {
    public:
        using ObjectT = std::variant<Args...>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class... Args>
    void Scribe<std::variant<Args...>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
        {
            auto index = object.index();
            format(index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptSaveValue(object, format);
        }
        else
        {
            std::size_t index;
            format(index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptLoadValue(index, object, format);
        }
    }

    template<class... Args>
    void Scribe<std::variant<Args...>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            auto output = format.AsOutput();
            output->StartObject(name);

            auto index = object.index();
            format("index", index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptSaveValue(object, format);

            output->EndObject();
        }
        else
        {
            auto input = format.AsInput();
            input->StartObject(name);

            std::size_t index;
            format("index", index);
            detail::UnpackVariant<sizeof...(Args)>::AttemptLoadValue(index, object, format);

            input->EndObject();
        }
    }

    template<class... Args, class Format>
    struct ScribeTraits<std::variant<Args...>, Format>
    {
        using Category = TrackingScribeCategory<std::variant<Args...>>;
    };
}