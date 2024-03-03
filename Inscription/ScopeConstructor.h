#pragma once

#include "Scribe.h"
#include "Storage.h"

namespace Inscription
{
    template<class T>
    class ScopeConstructor
    {
    public:
        template<class Archive>
        ScopeConstructor(Archive& archive);
        ~ScopeConstructor();

        T* operator->();
        const T* operator->() const;
        operator T*();
        operator const T*() const;

        T* Get();
        const T* Get() const;
        T&& GetMove();
    private:
        T* object;
    };

    template<class T>
    template<class Archive>
    ScopeConstructor<T>::ScopeConstructor(Archive& archive) : object(nullptr)
    {
        object = reinterpret_cast<T*>(CreateStorage(sizeof(T)));
        Scribe<T, Archive> scribe;
        scribe.Construct(object, archive);
    }

    template<class T>
    ScopeConstructor<T>::~ScopeConstructor()
    {
        delete object;
    }

    template<class T>
    T* ScopeConstructor<T>::operator->()
    {
        return object;
    }

    template<class T>
    const T* ScopeConstructor<T>::operator->() const
    {
        return object;
    }

    template<class T>
    ScopeConstructor<T>::operator T*()
    {
        return object;
    }

    template<class T>
    ScopeConstructor<T>::operator const T*() const
    {
        return object;
    }

    template<class T>
    T* ScopeConstructor<T>::Get()
    {
        return object;
    }

    template<class T>
    const T* ScopeConstructor<T>::Get() const
    {
        return object;
    }

    template<class T>
    T&& ScopeConstructor<T>::GetMove()
    {
        return std::move(*object);
    }
}