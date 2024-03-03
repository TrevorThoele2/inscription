#pragma once

namespace Inscription
{
    template<class T>
    class ScopedConstructor
    {
    public:
        template<class ScribeT>
        ScopedConstructor(ScribeT& scribe);
        ~ScopedConstructor();

        T* operator->();
        const T* operator->() const;
        operator T*();
        operator const T*() const;

        T* Get();
        const T* Get() const;
        T&& GetMove();
    private:
        T* obj;
    };

    template<class T>
    template<class ScribeT>
    ScopedConstructor<T>::ScopedConstructor(ScribeT& scribe) : obj(nullptr)
    {
        Construct(scribe, obj);
    }

    template<class T>
    ScopedConstructor<T>::~ScopedConstructor()
    {
        delete obj;
    }

    template<class T>
    T* ScopedConstructor<T>::operator->()
    {
        return obj;
    }

    template<class T>
    const T* ScopedConstructor<T>::operator->() const
    {
        return obj;
    }

    template<class T>
    ScopedConstructor<T>::operator T*()
    {
        return obj;
    }

    template<class T>
    ScopedConstructor<T>::operator const T*() const
    {
        return obj;
    }

    template<class T>
    T* ScopedConstructor<T>::Get()
    {
        return obj;
    }

    template<class T>
    const T* ScopedConstructor<T>::Get() const
    {
        return obj;
    }

    template<class T>
    T&& ScopedConstructor<T>::GetMove()
    {
        return std::move(*obj);
    }
}