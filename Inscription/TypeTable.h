#pragma once

#include <typeindex>
#include <unordered_map>
#include <map>

namespace Inscription
{
    template<class Container, class T>
    class TypeTableBase
    {
    private:
        typedef Container Indices;
    public:
        typedef typename Indices::iterator iterator;
        typedef typename Indices::const_iterator const_iterator;
        typedef typename Indices::size_type size_type;
    public:
        TypeTableBase() = default;
        TypeTableBase(const TypeTableBase& arg) = default;
        TypeTableBase(TypeTableBase&& arg);

        TypeTableBase& operator=(const TypeTableBase& arg) = default;
        TypeTableBase& operator=(TypeTableBase&& arg);

        std::pair<iterator, bool> Add(T& obj);
        std::pair<iterator, bool> Add(T&& obj);
        std::pair<iterator, bool> Add(T& obj, const std::type_info& info);
        std::pair<iterator, bool> Add(T&& obj, const std::type_info& info);
        std::pair<iterator, bool> Add(T& obj, const std::type_index& info);
        std::pair<iterator, bool> Add(T&& obj, const std::type_index& info);

        T* Find(const std::type_info& info);
        const T* Find(const std::type_info& info) const;
        T* Find(const std::type_index& index);
        const T* Find(const std::type_index& index) const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        size_type size() const;
        bool empty() const;
    private:
        Indices indices;
    };
    
    template<class Container, class T>
    TypeTableBase<Container, T>::TypeTableBase(TypeTableBase&& arg) : indices(std::move(arg.indices))
    {}

    template<class Container, class T>
    TypeTableBase<Container, T>& TypeTableBase<Container, T>::operator=(TypeTableBase&& arg)
    {
        indices = std::move(arg.indices);
        return *this;
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T& obj)
    {
        return indices.emplace(std::type_index(typeid(obj)), obj);
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T&& obj)
    {
        return indices.emplace(std::type_index(typeid(obj)), std::move(obj));
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T& obj, const std::type_info& info)
    {
        return indices.emplace(std::type_index(info), obj);
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T&& obj, const std::type_info& info)
    {
        return indices.emplace(std::type_index(info), std::move(obj));
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T& obj, const std::type_index& info)
    {
        return indices.emplace(info, obj);
    }

    template<class Container, class T>
    std::pair<typename TypeTableBase<Container, T>::iterator, bool> TypeTableBase<Container, T>::Add(T&& obj, const std::type_index& info)
    {
        return indices.emplace(info, std::move(obj));
    }

    template<class Container, class T>
    T* TypeTableBase<Container, T>::Find(const std::type_info& info)
    {
        return Find(std::type_index(info));
    }

    template<class Container, class T>
    const T* TypeTableBase<Container, T>::Find(const std::type_info& info) const
    {
        return Find(std::type_index(info));
    }

    template<class Container, class T>
    T* TypeTableBase<Container, T>::Find(const std::type_index& index)
    {
        auto possibleFound = indices.find(index);
        if(possibleFound != indices.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Container, class T>
    const T* TypeTableBase<Container, T>::Find(const std::type_index& index) const
    {
        auto possibleFound = indices.find(index);
        if (possibleFound != indices.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Container, class T>
    typename TypeTableBase<Container, T>::iterator TypeTableBase<Container, T>::begin()
    {
        return indices.begin();
    }

    template<class Container, class T>
    typename TypeTableBase<Container, T>::const_iterator TypeTableBase<Container, T>::begin() const
    {
        return indices.begin();
    }

    template<class Container, class T>
    typename TypeTableBase<Container, T>::iterator TypeTableBase<Container, T>::end()
    {
        return indices.end();
    }

    template<class Container, class T>
    typename TypeTableBase<Container, T>::const_iterator TypeTableBase<Container, T>::end() const
    {
        return indices.end();
    }

    template<class Container, class T>
    typename TypeTableBase<Container, T>::size_type TypeTableBase<Container, T>::size() const
    {
        return indices.size();
    }

    template<class Container, class T>
    bool TypeTableBase<Container, T>::empty() const
    {
        return indices.empty();
    }

    template<class T>
    using TypeTable = TypeTableBase<std::unordered_map<std::type_index, T>, T>;

    template<class T>
    using OrderedTypeTable = TypeTableBase<std::map<std::type_index, T>, T>;
}