
#pragma once

namespace Inscription
{
    class Scribe;
    template<class T>
    class StackConstructor
    {
    private:
        T *obj;
    public:
        StackConstructor(Scribe &scribe);
        ~StackConstructor();
        T* operator->();
        const T* operator->() const;
        operator T*();
        operator const T*() const;
        T* Get();
        const T* Get() const;
        T&& GetMove();
    };

    template<class T>
    StackConstructor<T>::StackConstructor(Scribe &scribe) : obj(nullptr)
    {
        Construct(scribe, obj);
    }

    template<class T>
    StackConstructor<T>::~StackConstructor()
    {
        delete obj;
    }

    template<class T>
    T* StackConstructor<T>::operator->()
    {
        return obj;
    }

    template<class T>
    const T* StackConstructor<T>::operator->() const
    {
        return obj;
    }

    template<class T>
    StackConstructor<T>::operator T*()
    {
        return obj;
    }

    template<class T>
    StackConstructor<T>::operator const T*() const
    {
        return obj;
    }

    template<class T>
    T* StackConstructor<T>::Get()
    {
        return obj;
    }

    template<class T>
    const T* StackConstructor<T>::Get() const
    {
        return obj;
    }

    template<class T>
    T&& StackConstructor<T>::GetMove()
    {
        return std::move(*obj);
    }
}