
#pragma once

#include <type_traits>
#include "Inscripter.h"
#include "IsDefaultConstructible.h"

namespace Inscription
{
    class Scribe;
    template<class T>
    class InscripterBase;

    // Serialize
    template<class T>
    void Serialize(Scribe& scribe, T& obj);

    // Base Serialize
    template<class BaseT, class T>
    void BaseSerialize(Scribe& scribe, T& obj);

    // Construct
    template<class T>
    void Construct(Scribe& scribe, T*& obj);

    template<class T>
    typename InscripterBase<T>::TableT MakeAndSerializeTable(Scribe& scribe, typename InscripterBase<T>::ManagedT& obj);

    template<class T>
    typename InscripterBase<T>::TableT MakeAndConstructTable(Scribe& scribe);

    class Access
    {
    private:
        template<class T>
        struct Void { typedef void type; };

        template <class T, class = void>
        struct HasInternalInscripter : std::false_type
        {};

#ifdef INSCRIPTION_CLASS_INSCRIPTER_SYMBOL
        template <class T>
        struct HasInternalInscripter<T, typename Void<typename T::INSCRIPTION_CLASS_INSCRIPTER_SYMBOL>::type> : std::true_type
        {};
#else
        template <class T>
        struct HasInternalInscripter<T, typename Void<typename T::InscripterT>::type> : std::true_type
        {};
#endif

        template <class T, class>
        struct DiscoverInscripterTypedef;

        template <class T>
        struct DiscoverInscripterTypedef<T, std::false_type>
        {
            typedef Inscripter<T> InscripterT;
        };

#ifdef INSCRIPTION_CLASS_INSCRIPTER_SYMBOL
        template <class T>
        struct DiscoverInscripterTypedef<T, std::true_type>
        {
            typedef T::INSCRIPTION_CLASS_INSCRIPTER_SYMBOL InscripterT;
        };
#else
        template <class T>
        struct DiscoverInscripterTypedef<T, std::true_type>
        {
            typedef typename T::InscripterT InscripterT;
        };
#endif
    public:
        template<class T>
        using InscripterT = typename DiscoverInscripterTypedef<T, std::integral_constant<bool, HasInternalInscripter<T>::value>>::InscripterT;

        template <class T, class = int>
        struct HasPostConstruct : std::false_type
        {};

        template <class T>
        struct HasPostConstruct<T, decltype((void)InscripterT<T>::PostConstruct, 0)> : std::true_type
        {};

        template <class T, class = int>
        struct HasClassNameResolver : std::false_type
        {};

        template <class T>
        struct HasClassNameResolver<T, decltype((void)InscripterT<T>::classNameResolver, 0)> : std::true_type
        {};

        template<class T, typename std::enable_if<!InscripterT<T>::exists, int>::type = 0>
        static void Serialize(Scribe& scribe, T& obj)
        {
#ifdef INSCRIPTION_SERIALIZE_SYMBOL
            obj.INSCRIPTION_SERIALIZE_SYMBOL(scribe);
#else
            obj.Serialize(scribe);
#endif
        }

        template<class T, typename std::enable_if<InscripterT<T>::exists, int>::type = 0>
        static void Serialize(Scribe& scribe, T& obj)
        {
            MakeAndSerializeTable<T>(scribe, obj);
        }

        template<class T, class ScribeT = Scribe, typename std::enable_if<!InscripterT<T>::exists, int>::type = 0>
        static void Construct(ScribeT& scribe, T*& obj)
        {
            static_assert(std::is_same<ScribeT, Scribe>::value, "This type must be a scribe.");
            static_assert(IsDefaultConstructible<T>::value, "Non-default constructible objects must have an inscripter to be constructed.");

            obj = new T();
            scribe(*obj);
        }

        template<class T, class ScribeT = Scribe, typename std::enable_if<InscripterT<T>::exists, int>::type = 0>
        static void Construct(ScribeT& scribe, T*& obj)
        {
            static_assert(std::is_same<ScribeT, Scribe>::value, "This type must be a scribe.");

            InscripterT<T>::ConstructObjectInstance(scribe, obj);
            InscripterT<T>::PostConstruct(*obj);
            scribe.TrackObject(obj);
        }
    };

    template<class T>
    void Serialize(Scribe& scribe, T& obj)
    {
        Access::Serialize(scribe, obj);
    }

    template<class BaseT, class T>
    void BaseSerialize(Scribe& scribe, T& obj)
    {
        static_assert(!std::is_same<BaseT, T>::value, "A type has been attempted to be converted to itself and serialized.");
        Access::Serialize(scribe, static_cast<BaseT&>(obj));
    }

    template<class T>
    void Construct(Scribe& scribe, T*& obj)
    {
        Access::Construct(scribe, obj);
    }
}