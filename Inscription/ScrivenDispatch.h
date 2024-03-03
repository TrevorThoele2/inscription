#pragma once

#include "Scribe.h"
#include "HasCategoryTrait.h"
#include "RequiresScribe.h"
#include "Const.h"

namespace Inscription
{
    namespace Archive
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
        template<class T, class Archive>
        using KnownScribeTraits = ScribeTraits<KnownType<T>, Archive>;
        template<class T, class Archive>
        using KnownScribeCategory = typename KnownScribeTraits<T, Archive>::Category;

        template<class T, class Archive>
        using CategoryAndScribeExecute =
            std::conjunction<
                has_category_trait<KnownScribeTraits<T, Archive>>,
                requires_scribe<KnownScribeCategory<T, Archive>>>;
        template<class T, class Archive>
        using CategoryExecute =
            std::conjunction<
                has_category_trait<KnownScribeTraits<T, Archive>>,
                std::negation<requires_scribe<KnownScribeCategory<T, Archive>>>>;
        template<class T, class Archive>
        using ScribeExecute = std::negation<has_category_trait<KnownScribeTraits<T, Archive>>>;
    public:
        template<class T, class Archive, std::enable_if_t<CategoryAndScribeExecute<T, Archive>::value, int> = 0>
        static void Execute(T& object, Archive& archive)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;

            using CategoryT = KnownScribeCategory<T, Archive>;
            CategoryT::Scriven(RemoveConst(object), archive, scribe);
        }

        template<class T, class Archive, std::enable_if_t<CategoryExecute<T, Archive>::value, int> = 0>
        static void Execute(T& object, Archive& archive)
        {
            using CategoryT = KnownScribeCategory<T, Archive>;
            CategoryT::Scriven(RemoveConst(object), archive);
        }

        template<class T, class Archive, std::enable_if_t<ScribeExecute<T, Archive>::value, int> = 0>
        static void Execute(T& object, Archive& archive)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;
            scribe.Scriven(RemoveConst(object), archive);
        }

        template<class T>
        static void Execute(T& object, Archive::Json& archive)
        {
            NamedExecute("", object, archive);
        }

        template<class T, class Archive, std::enable_if_t<CategoryAndScribeExecute<T, Archive>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Archive& archive)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;

            using CategoryT = KnownScribeCategory<T, Archive>;
            CategoryT::Scriven(name, RemoveConst(object), archive, scribe);
        }

        template<class T, class Archive, std::enable_if_t<CategoryExecute<T, Archive>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Archive& archive)
        {
            using CategoryT = KnownScribeCategory<T, Archive>;
            CategoryT::Scriven(name, RemoveConst(object), archive);
        }

        template<class T, class Archive, std::enable_if_t<ScribeExecute<T, Archive>::value, int> = 0>
        static void NamedExecute(const std::string& name, T& object, Archive& archive)
        {
            using ScribeT = KnownScribe<T>;
            ScribeT scribe;
            scribe.Scriven(name, RemoveConst(object), archive);
        }
    };

    template<class BaseT, class T, class Archive>
    void BaseScriven(T& object, Archive& archive)
    {
        ScrivenDispatch::Execute(static_cast<BaseT&>(RemoveConst(object)), archive);
    }
}