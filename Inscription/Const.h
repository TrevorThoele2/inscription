#pragma once

namespace Inscription
{
    template<class T>
    struct is_const : std::false_type
    {};

    template<class T>
    struct is_const<const T> : std::true_type
    {};

    template<class T>
    struct is_const<const T*> : std::true_type
    {};

    template<class T>
    struct is_const<const T&> : std::true_type
    {};

    template<class T>
    static constexpr bool is_const_v = is_const<T>::type;

    template<class T>
    struct remove_const
    {
        using type = T;
    };

    template<class T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template<class T>
    struct remove_const<const T*>
    {
        using type = T*;
    };

    template<class T>
    struct remove_const<T* const>
    {
        using type = T*;
    };

    template<class T>
    struct remove_const<const T* const>
    {
        using type = T*;
    };

    template<class T>
    struct remove_const<const T&>
    {
        using type = T&;
    };

    template<class T>
    using remove_const_t = typename remove_const<T>::type;

    template<class T>
    remove_const_t<T>& RemoveConst(T& object)
    {
        return const_cast<remove_const_t<T>&>(object);
    }

    template<class T>
    remove_const_t<T>&& RemoveConst(T&& object)
    {
        return const_cast<remove_const_t<T>&&>(object);
    }
}