#pragma once

#include <tuple>

#include "TrackingScribeCategory.h"

#include "ScopeConstructor.h"

#include "OutputJsonFormat.h"
#include "InputJsonFormat.h"

namespace Inscription
{
    namespace detail
    {
        template<unsigned int I>
        class UnpackTuple
        {
        public:
            template<class... Args>
            static void Scriven(std::tuple<Args...>& object, Format::Binary& format)
            {
                auto& value = std::get<I - 1>(object);
                format(value);
                UnpackTuple<I - 1>::Scriven(object, format);
            }

            template<class... Args>
            static void Scriven(std::tuple<Args...>& object, size_t& i, Format::Json& format)
            {
                auto& value = std::get<I - 1>(object);
                format(Chroma::ToString(i), value);
                ++i;
                UnpackTuple<I - 1>::Scriven(object, i, format);
            }
        };

        template<>
        class UnpackTuple<0>
        {
        public:
            template<class... Args>
            static void Scriven(std::tuple<Args...>&, Format::Binary&)
            {}

            template<class... Args>
            static void Scriven(std::tuple<Args...>&, size_t& i, Format::Json&)
            {}
        };
    }

    template<class... Args>
    class Scribe<std::tuple<Args...>> final
    {
    public:
        using ObjectT = std::tuple<Args...>;
    public:
        void Scriven(ObjectT& object, Format::Binary& format);
        void Scriven(const std::string& name, ObjectT& object, Format::Json& format);
    };

    template<class... Args>
    void Scribe<std::tuple<Args...>>::Scriven(ObjectT& object, Format::Binary& format)
    {
        detail::UnpackTuple<sizeof...(Args)>::Scriven(object, format);
    }

    template<class... Args>
    void Scribe<std::tuple<Args...>>::Scriven(const std::string& name, ObjectT& object, Format::Json& format)
    {
        if (format.IsOutput())
        {
            const auto output = format.AsOutput();
            output->StartList(name);
            size_t i = 0;
            detail::UnpackTuple<sizeof...(Args)>::Scriven(object, i, format);
            output->EndList();
        }
        else
        {
            const auto input = format.AsInput();
            input->StartList(name);
            size_t i = 0;
            detail::UnpackTuple<sizeof...(Args)>::Scriven(object, i, format);
            input->EndList();
        }
    }

    template<class... Args, class Format>
    struct ScribeTraits<std::tuple<Args...>, Format>
    {
        using Category = TrackingScribeCategory<std::tuple<Args...>>;
    };
}