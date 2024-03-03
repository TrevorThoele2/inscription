#pragma once

#include "Scribe.h"
#include "Storage.h"
#include "Access.h"

namespace Inscription
{
    template<class Object>
    class ScopeConstructor
    {
    public:
        template<class Archive>
        explicit ScopeConstructor(Archive& archive);
        ~ScopeConstructor();

        Object* operator->();
        const Object* operator->() const;
        operator Object*();
        operator const Object*() const;

        Object* Get();
        const Object* Get() const;
        Object&& GetMove();
    private:
        Object* object;
    };

    template<class Object>
    template<class Archive>
    ScopeConstructor<Object>::ScopeConstructor(Archive& archive) : object(nullptr)
    {
        object = reinterpret_cast<Object*>(CreateStorage(sizeof(Object)));
        Scribe<Object, Archive> scribe;
        Access::Construct(object, archive, scribe);
    }

    template<class Object>
    ScopeConstructor<Object>::~ScopeConstructor()
    {
        delete object;
    }

    template<class Object>
    Object* ScopeConstructor<Object>::operator->()
    {
        return object;
    }

    template<class Object>
    const Object* ScopeConstructor<Object>::operator->() const
    {
        return object;
    }

    template<class Object>
    ScopeConstructor<Object>::operator Object*()
    {
        return object;
    }

    template<class Object>
    ScopeConstructor<Object>::operator const Object*() const
    {
        return object;
    }

    template<class Object>
    Object* ScopeConstructor<Object>::Get()
    {
        return object;
    }

    template<class Object>
    const Object* ScopeConstructor<Object>::Get() const
    {
        return object;
    }

    template<class Object>
    Object&& ScopeConstructor<Object>::GetMove()
    {
        return std::move(*object);
    }
}