#pragma once

#include <memory>
#include <tuple>

#include "Access.h"
#include "ScopedConstructor.h"
#include "Const.h"

namespace Inscription
{
    namespace detail
    {
        template<unsigned int I>
        class UnpackTuple
        {
        private:
            template<class ScribeT, class T>
            inline static void SerializeSingle(ScribeT& scribe, T& obj)
            {
                scribe(obj);
            }

            template<class ScribeT, class T>
            inline static void SerializeSingle(ScribeT& scribe, const T& obj)
            {
                scribe(RemoveConst(obj));
            }

            template<class ScribeT, class T, typename std::enable_if<!IsConst<T>::value, int>::type = 0>
            inline static std::unique_ptr<T> ConstructSingle(ScribeT& scribe)
            {
                typedef std::unique_ptr<T> Ret;

                T* obj = nullptr;
                ::inscription::Construct(scribe, obj);
                return Ret(obj);
            }

            template<class ScribeT, class T, typename std::enable_if<IsConst<T>::value, int>::type = 0>
            inline static std::unique_ptr<typename RemoveConstTrait<T>::type> ConstructSingle(ScribeT& scribe)
            {
                typedef RemoveConstTrait<T>::type Type;
                typedef std::unique_ptr<Type> Ret;

                Type* obj = nullptr;
                ::inscription::Construct(scribe, obj);
                return Ret(obj);
            }
        public:
            template<class ScribeT, class... Args>
            inline static void SerializeGroup(ScribeT& scribe, std::tuple<Args...>& argTuple)
            {
                SerializeSingle(scribe, std::get<I - 1>(argTuple));
                UnpackTuple<I - 1>::SerializeGroup(scribe, argTuple);
            }

            template<class ScribeT, class... Args, class... Holder>
            inline static void ConstructGroup(ScribeT& scribe, std::tuple<Args...>*& obj, Holder&& ... holder)
            {
                typedef std::tuple<Args...> Tuple;
                auto ptr = ConstructSingle<std::tuple_element<I - 1, Tuple>::type>(scribe);
                UnpackTuple<I - 1>::ConstructGroup(scribe, obj, *ptr.get(), std::forward<Holder>(holder)...);
            }
        };

        template<>
        class UnpackTuple<0>
        {
        public:
            template<class ScribeT, class... Args>
            inline static void SerializeGroup(ScribeT& scribe, std::tuple<Args...>& argTuple)
            {}

            template<class ScribeT, class... Args, class... Holder>
            inline static void ConstructGroup(ScribeT& scribe, std::tuple<Args...>*& obj, Holder&& ... holder)
            {
                obj = new std::tuple<Args...>(std::forward<Holder>(holder)...);
            }
        };
    }

    template<class ScribeT, class... Args>
    void Serialize(ScribeT& scribe, std::tuple<Args...>& arg)
    {
        detail::UnpackTuple<sizeof...(Args)>::SerializeGroup(scribe, arg);
    }

    template<class ScribeT, class... Args>
    void Construct(ScribeT& scribe, std::tuple<Args...>*& obj)
    {
        detail::UnpackTuple<sizeof...(Args)>::ConstructGroup(scribe, obj);
    }
}