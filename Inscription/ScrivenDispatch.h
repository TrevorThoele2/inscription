#pragma once

#include <string>
#include "Scribe.h"
#include "HasCategoryTrait.h"
#include "RequiresScribe.h"
#include "Const.h"

namespace Inscription
{
    namespace Format
    {
        class Json;
    }

    class ScrivenDispatch
    {
    private:
        template<class T>
        using KnownType = remove_const_t<T>;
        template<class T>
        using KnownScribe = Scribe<KnownType<T>>;
        template<class T, class Format>
        using KnownScribeTraits = ScribeTraits<KnownType<T>, Format>;
        template<class T, class Format>
        using KnownScribeCategory = typename KnownScribeTraits<T, Format>::Category;

        template<class T, class Format>
        using CategoryAndScribeExecute =
            std::conjunction<
                has_category_trait<KnownScribeTraits<T, Format>>,
                requires_scribe<KnownScribeCategory<T, Format>>>;
        template<class T, class Format>
        using CategoryExecute =
            std::conjunction<
                has_category_trait<KnownScribeTraits<T, Format>>,
                std::negation<requires_scribe<KnownScribeCategory<T, Format>>>>;
        template<class T, class Format>
        using ScribeExecute = std::negation<has_category_trait<KnownScribeTraits<T, Format>>>;
    public:
        template<class T, class Format, std::enable_if_t<CategoryAndScribeExecute<T, Format>::value, int> = 0>
        static void Execute(T& object, Format& format)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;

            using CategoryT = KnownScribeCategory<T, Format>;
            CategoryT::Scriven(RemoveConst(object), format, scribe);
        }

        template<class T, class Format, std::enable_if_t<CategoryExecute<T, Format>::value, int> = 0>
        static void Execute(T& object, Format& format)
        {
            using CategoryT = KnownScribeCategory<T, Format>;
            CategoryT::Scriven(RemoveConst(object), format);
        }

        template<class T, class Format, std::enable_if_t<ScribeExecute<T, Format>::value, int> = 0>
        static void Execute(T& object, Format& format)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;
            scribe.Scriven(RemoveConst(object), format);
        }

        template<class T>
        static void Execute(T& object, Format::Json& format)
        {
            NamedExecute("", object, format);
        }

        template<class T, class Format, std::enable_if_t<CategoryAndScribeExecute<T, Format>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Format& format)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;

            using CategoryT = KnownScribeCategory<T, Format>;
            CategoryT::Scriven(name, RemoveConst(object), format, scribe);
        }

        template<class T, class Format, std::enable_if_t<CategoryExecute<T, Format>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Format& format)
        {
            using CategoryT = KnownScribeCategory<T, Format>;
            CategoryT::Scriven(name, RemoveConst(object), format);
        }

        template<class T, class Format, std::enable_if_t<ScribeExecute<T, Format>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Format& format)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;
            scribe.Scriven(name, RemoveConst(object), format);
        }
    };

    template<class BaseT, class T, class Format>
    void BaseScriven(T& object, Format& format)
    {
        ScrivenDispatch::Execute(static_cast<BaseT&>(RemoveConst(object)), format);
    }
}