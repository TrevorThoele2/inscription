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
        template<class Format>
        explicit ScopeConstructor(Format& format);
        ScopeConstructor(const std::string& name, Format::Json& format);
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
    template<class Format>
    ScopeConstructor<Object>::ScopeConstructor(Format& format) : object(nullptr)
    {
        object = reinterpret_cast<Object*>(CreateStorage(sizeof(Object)));
        ConstructDispatch::Execute(object, format);
    }

    template<class Object>
    ScopeConstructor<Object>::ScopeConstructor(const std::string& name, Format::Json& format)
    {
        object = reinterpret_cast<Object*>(CreateStorage(sizeof(Object)));
        ConstructDispatch::NamedExecute(name, object, format);
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