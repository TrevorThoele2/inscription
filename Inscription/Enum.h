#pragma once

#include <type_traits>

namespace inscription
{
    template<class EnumT>
    typename std::underlying_type<EnumT>::type CastEnumToUnderlying(EnumT e)
    {
        return static_cast<typename std::underlying_type<EnumT>::type>(e);
    }

    template<class EnumT>
    typename std::underlying_type<EnumT>::type& CastEnumToUnderlyingRef(EnumT &e)
    {
        return reinterpret_cast<typename std::underlying_type<EnumT>::type&>(e);
    }

    struct HashEnum
    {
        template<class EnumT>
        std::size_t operator()(EnumT e) const
        {
            return static_cast<std::size_t>(e);
        }
    };
}