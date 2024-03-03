
#pragma once

#include <memory>
#include <tuple>

#include "Access.h"
#include "StackConstructor.h"
#include "Const.h"

namespace Inscription
{
    class Scribe;
    namespace detail
    {
        template<unsigned int I>
        class UnpackTuple
        {
        private:
            template<class T>
            inline static void SerializeSingle(Scribe &scribe, T &obj)
            {
                scribe(obj);
            }

            template<class T>
            inline static void SerializeSingle(Scribe &scribe, const T &obj)
            {
                scribe(RemoveConst(obj));
            }

            template<class T, typename std::enable_if<!IsConst<T>::value, int>::type = 0>
            inline static std::unique_ptr<T> ConstructSingle(Scribe &scribe)
            {
                typedef std::unique_ptr<T> Ret;

                T *obj = nullptr;
                ::inscription::Construct(scribe, obj);
                return Ret(obj);
            }

            template<class T, typename std::enable_if<IsConst<T>::value, int>::type = 0>
            inline static std::unique_ptr<typename RemoveConstTrait<T>::type> ConstructSingle(Scribe &scribe)
            {
                typedef RemoveConstTrait<T>::type Type;
                typedef std::unique_ptr<Type> Ret;

                Type *obj = nullptr;
                ::inscription::Construct(scribe, obj);
                return Ret(obj);
            }
        public:
            template<class... Args>
            inline static void SerializeGroup(Scribe &scribe, std::tuple<Args...> &argTuple)
            {
                SerializeSingle(scribe, std::get<I - 1>(argTuple));
                UnpackTuple<I - 1>::SerializeGroup(scribe, argTuple);
            }

            template<class... Args, class... Holder>
            inline static void ConstructGroup(Scribe &scribe, std::tuple<Args...> *&obj, Holder && ... holder)
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
            template<class... Args>
            inline static void SerializeGroup(Scribe &scribe, std::tuple<Args...> &argTuple)
            {}

            template<class... Args, class... Holder>
            inline static void ConstructGroup(Scribe &scribe, std::tuple<Args...> *&obj, Holder && ... holder)
            {
                obj = new std::tuple<Args...>(std::forward<Holder>(holder)...);
            }
        };
    }

    template<class... Args>
    void Serialize(Scribe &scribe, std::tuple<Args...> &arg)
    {
        detail::UnpackTuple<sizeof...(Args)>::SerializeGroup(scribe, arg);
    }

    template<class... Args>
    void Construct(Scribe &scribe, std::tuple<Args...> *&obj)
    {
        inscription::detail::UnpackTuple<sizeof...(Args)>::ConstructGroup(scribe, obj);
    }
}