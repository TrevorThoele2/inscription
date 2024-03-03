#pragma once

#include <memory>
#include <vector>
#include <typeindex>

#include "PointerManager.h"
#include "TrackerMap.h"
#include "Access.h"

namespace Inscription
{
    template<class ScribeT>
    class RegisteredTypes
    {
    public:
        RegisteredTypes() = default;

        template<class T>
        void Register();

        void CopyRegisteredTo(TrackerMap& group) const;
        void CopyRegisteredTo(PointerManager<ScribeT>& manager, ScribeT& scribe) const;

        template<class T>
        bool Has() const;
    private:
        class Base
        {
        public:
            virtual ~Base() = 0;

            virtual std::type_index Type() const = 0;
            virtual void FillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe) const = 0;
        };

        template<class T, typename std::enable_if<!std::is_abstract<T>::value && Access::InscripterT<T>::exists, int>::type = 0>
        static void DelegateFillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe);
        template<class T, typename std::enable_if<std::is_abstract<T>::value || !Access::InscripterT<T>::exists, int>::type = 0>
        static void DelegateFillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe);

        template<class T>
        class Derived : public Base
        {
        public:
            std::type_index Type() const override;
            void FillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe) const override;
        };

        typedef std::unique_ptr<Base> BasePtr;
        typedef std::vector<BasePtr> BaseList;
        BaseList bases;
    };

    template<class ScribeT>
    template<class T>
    void RegisteredTypes<ScribeT>::Register()
    {
        if (Has<T>())
            return;

        bases.push_back(std::make_unique<Derived<T>>());
    }

    template<class ScribeT>
    void RegisteredTypes<ScribeT>::CopyRegisteredTo(TrackerMap& group) const
    {
        for (auto& loop : bases)
            group.table.Add(Tracker(), loop->Type());
    }

    template<class ScribeT>
    void RegisteredTypes<ScribeT>::CopyRegisteredTo(PointerManager<ScribeT>& manager, ScribeT& scribe) const
    {
        for (auto& loop : bases)
            loop->FillPolymorphic(manager, scribe);
    }

    template<class ScribeT>
    template<class T>
    bool RegisteredTypes<ScribeT>::Has() const
    {
        auto type = std::type_index(typeid(T));
        for (auto& loop : bases)
            if (loop->Type() == type)
                return true;

        return false;
    }

    template<class ScribeT>
    RegisteredTypes<ScribeT>::Base::~Base()
    {}

    template<class ScribeT>
    template<class T, typename std::enable_if<!std::is_abstract<T>::value && Access::InscripterT<T>::exists, int>::type>
    void RegisteredTypes<ScribeT>::DelegateFillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe)
    {
        manager.Add<T>(Inscripter<T>::classNameResolver.NameFor(scribe));
    }

    template<class ScribeT>
    template<class T, typename std::enable_if<std::is_abstract<T>::value || !Access::InscripterT<T>::exists, int>::type>
    void RegisteredTypes<ScribeT>::DelegateFillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe)
    {}

    template<class ScribeT>
    template<class T>
    std::type_index RegisteredTypes<ScribeT>::Derived<T>::Type() const
    {
        return std::type_index(typeid(T));
    }

    template<class ScribeT>
    template<class T>
    void RegisteredTypes<ScribeT>::Derived<T>::FillPolymorphic(PointerManager<ScribeT>& manager, ScribeT& scribe) const
    {
        DelegateFillPolymorphic<T>(manager, scribe);
    }
}