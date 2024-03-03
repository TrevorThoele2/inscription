#pragma once

#include <tuple>

#include "TrackingScribeCategory.h"

#include "ScopeConstructor.h"

#include "OutputJsonArchive.h"
#include "InputJsonArchive.h"

namespace Inscription
{
    namespace detail
    {
        template<unsigned int I>
        class UnpackTuple
        {
        public:
            template<class... Args>
            static void Scriven(std::tuple<Args...>& object, Archive::Binary& archive)
            {
                auto& value = std::get<I - 1>(object);
                archive(value);
                UnpackTuple<I - 1>::Scriven(object, archive);
            }

            template<class... Args>
            static void Scriven(std::tuple<Args...>& object, Archive::Json& archive)
            {
                auto& value = std::get<I - 1>(object);
                archive("", value);
                UnpackTuple<I - 1>::Scriven(object, archive);
            }
        };

        template<>
        class UnpackTuple<0>
        {
        public:
            template<class... Args>
            static void Scriven(std::tuple<Args...>&, Archive::Binary&)
            {}

            template<class... Args>
            static void Scriven(std::tuple<Args...>&, Archive::Json&)
            {}
        };
    }

    template<class... Args>
    class Scribe<std::tuple<Args...>> final
    {
    public:
        using ObjectT = std::tuple<Args...>;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive);
        void Scriven(const std::string& name, ObjectT& object, Archive::Json& archive);
    };

    template<class... Args>
    void Scribe<std::tuple<Args...>>::Scriven(ObjectT& object, Archive::Binary& archive)
    {
        detail::UnpackTuple<sizeof...(Args)>::Scriven(object, archive);
    }

    template<class... Args>
    void Scribe<std::tuple<Args...>>::Scriven(const std::string& name, ObjectT& object, Archive::Json& archive)
    {
        if (archive.IsOutput())
        {
            auto output = archive.AsOutput();
            output->StartList(name);
            detail::UnpackTuple<sizeof...(Args)>::Scriven(object, archive);
            output->EndList();
        }
        else
        {
            auto input = archive.AsInput();
            input->StartList(name);
            detail::UnpackTuple<sizeof...(Args)>::Scriven(object, archive);
            input->EndList();
        }
    }

    template<class... Args, class Archive>
    struct ScribeTraits<std::tuple<Args...>, Archive>
    {
        using Category = TrackingScribeCategory<std::tuple<Args...>>;
    };
}