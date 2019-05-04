#pragma once

#include <memory>
#include <vector>
#include <typeindex>

#include "Access.h"

namespace Inscription
{
    class TrackerGroup;
    class PointerManager;
    class Scribe;
    template<class T>
    class Inscripter;

    class RegisteredTypes
    {
    public:
        template<class T>
        static void Register();

        static void PushToTracking(TrackerGroup& group);
        static void PushToPolymorphic(PointerManager& manager, Scribe& scribe);

        template<class T>
        static bool Has();
    private:
        RegisteredTypes() = default;

        static RegisteredTypes& Instance();
    private:
        class Base
        {
        public:
            virtual ~Base() = 0;

            virtual std::type_index Type() const = 0;
            virtual void FillPolymorphic(PointerManager& manager, Scribe& scribe) const = 0;
        };

        template<class T, typename std::enable_if<!std::is_abstract<T>::value && Access::InscripterT<T>::exists, int>::type = 0>
        static void DelegateFillPolymorphic(PointerManager& manager, Scribe& scribe);
        template<class T, typename std::enable_if<std::is_abstract<T>::value || !Access::InscripterT<T>::exists, int>::type = 0>
        static void DelegateFillPolymorphic(PointerManager& manager, Scribe& scribe);

        template<class T>
        class Derived : public Base
        {
        public:
            std::type_index Type() const override;
            void FillPolymorphic(PointerManager& manager, Scribe& scribe) const override;
        };

        typedef std::unique_ptr<Base> BasePtr;
        typedef std::vector<BasePtr> BaseList;
        BaseList bases;
    };

    template<class T>
    void RegisteredTypes::Register()
    {
        if (Has<T>())
            return;

        Instance().bases.push_back(std::make_unique<Derived<T>>());
    }

    template<class T>
    bool RegisteredTypes::Has()
    {
        auto type = std::type_index(typeid(T));
        for (auto& loop : Instance().bases)
            if (loop->Type() == type)
                return true;

        return false;
    }

    template<class T, typename std::enable_if<!std::is_abstract<T>::value && Access::InscripterT<T>::exists, int>::type>
    void RegisteredTypes::DelegateFillPolymorphic(PointerManager& manager, Scribe& scribe)
    {
        manager.Add<T>(Inscripter<T>::classNameResolver.NameFor(scribe));
    }

    template<class T, typename std::enable_if<std::is_abstract<T>::value || !Access::InscripterT<T>::exists, int>::type>
    void RegisteredTypes::DelegateFillPolymorphic(PointerManager& manager, Scribe& scribe)
    {}

    template<class T>
    std::type_index RegisteredTypes::Derived<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class T>
    void RegisteredTypes::Derived<T>::FillPolymorphic(PointerManager& manager, Scribe& scribe) const
    {
        DelegateFillPolymorphic<T>(manager, scribe);
    }
}