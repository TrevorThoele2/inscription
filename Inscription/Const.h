
#pragma once

namespace Inscription
{
    template<class T>
    struct IsConst : public std::false_type {};

    template<class T>
    struct IsConst<const T> : public std::true_type {};

    template<class T>
    struct IsConst<const T*> : public std::true_type {};

    template<class T>
    struct IsConst<const T&> : public std::true_type {};

    template<class T>
    struct RemoveConstTrait
    {
        typedef T type;
    };

    template<class T>
    struct RemoveConstTrait<const T>
    {
        typedef T type;
    };

    template<class T>
    struct RemoveConstTrait<const T*>
    {
        typedef T* type;
    };

    template<class T>
    struct RemoveConstTrait<T* const>
    {
        typedef T* type;
    };

    template<class T>
    struct RemoveConstTrait<const T* const>
    {
        typedef T* type;
    };

    template<class T>
    struct RemoveConstTrait<const T&>
    {
        typedef T& type;
    };

    template<class T>
    inline typename RemoveConstTrait<T>::type&& RemoveConst(T &&obj)
    {
        return const_cast<typename RemoveConstTrait<T>::type&&>(obj);
    }

    template<class T>
    inline typename RemoveConstTrait<T>::type& RemoveConst(T &obj)
    {
        return const_cast<typename RemoveConstTrait<T>::type&>(obj);
    }
}