#pragma once

namespace Inscription
{
    template<class T>
    struct IsConst : std::false_type
    {};

    template<class T>
    struct IsConst<const T> : std::true_type
    {};

    template<class T>
    struct IsConst<const T*> : std::true_type
    {};

    template<class T>
    struct IsConst<const T&> : std::true_type
    {};

    template<class T>
    struct RemoveConstTrait
    {
        using type = T;
    };

    template<class T>
    struct RemoveConstTrait<const T>
    {
        using type = T;
    };

    template<class T>
    struct RemoveConstTrait<const T*>
    {
        using type = T*;
    };

    template<class T>
    struct RemoveConstTrait<T* const>
    {
        using type = T*;
    };

    template<class T>
    struct RemoveConstTrait<const T* const>
    {
        using type = T*;
    };

    template<class T>
    struct RemoveConstTrait<const T&>
    {
        using type = T&;
    };

    template<class T>
    typename RemoveConstTrait<T>::type&& RemoveConst(T&& object)
    {
        return const_cast<typename RemoveConstTrait<T>::type&&>(object);
    }

    template<class T>
    typename RemoveConstTrait<T>::type& RemoveConst(T& object)
    {
        return const_cast<typename RemoveConstTrait<T>::type&>(object);
    }
}