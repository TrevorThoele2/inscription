#pragma once

#include <typeindex>
#include <unordered_map>
#include <map>

namespace Inscription
{
    template<class Container, class T>
    class TypeMapBase
    {
    private:
        using Indices = Container;
    public:
        using iterator = typename Indices::iterator;
        using const_iterator = typename Indices::const_iterator;
        using size_type = typename Indices::size_type;
    public:
        TypeMapBase() = default;
        TypeMapBase(const TypeMapBase& arg) = default;
        TypeMapBase(TypeMapBase&& arg);

        TypeMapBase& operator=(const TypeMapBase& arg) = default;
        TypeMapBase& operator=(TypeMapBase&& arg);

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
    TypeMapBase<Container, T>::TypeMapBase(TypeMapBase&& arg) : indices(std::move(arg.indices))
    {}

    template<class Container, class T>
    TypeMapBase<Container, T>& TypeMapBase<Container, T>::operator=(TypeMapBase&& arg)
    {
        indices = std::move(arg.indices);
        return *this;
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T& obj)
    {
        return indices.emplace(std::type_index(typeid(obj)), obj);
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T&& obj)
    {
        return indices.emplace(std::type_index(typeid(obj)), std::move(obj));
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T& obj, const std::type_info& info)
    {
        return indices.emplace(std::type_index(info), obj);
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T&& obj, const std::type_info& info)
    {
        return indices.emplace(std::type_index(info), std::move(obj));
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T& obj, const std::type_index& info)
    {
        return indices.emplace(info, obj);
    }

    template<class Container, class T>
    std::pair<typename TypeMapBase<Container, T>::iterator, bool> TypeMapBase<Container, T>::Add(T&& obj, const std::type_index& info)
    {
        return indices.emplace(info, std::move(obj));
    }

    template<class Container, class T>
    T* TypeMapBase<Container, T>::Find(const std::type_info& info)
    {
        return Find(std::type_index(info));
    }

    template<class Container, class T>
    const T* TypeMapBase<Container, T>::Find(const std::type_info& info) const
    {
        return Find(std::type_index(info));
    }

    template<class Container, class T>
    T* TypeMapBase<Container, T>::Find(const std::type_index& index)
    {
        auto possibleFound = indices.find(index);
        if(possibleFound != indices.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Container, class T>
    const T* TypeMapBase<Container, T>::Find(const std::type_index& index) const
    {
        auto possibleFound = indices.find(index);
        if (possibleFound != indices.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Container, class T>
    typename TypeMapBase<Container, T>::iterator TypeMapBase<Container, T>::begin()
    {
        return indices.begin();
    }

    template<class Container, class T>
    typename TypeMapBase<Container, T>::const_iterator TypeMapBase<Container, T>::begin() const
    {
        return indices.begin();
    }

    template<class Container, class T>
    typename TypeMapBase<Container, T>::iterator TypeMapBase<Container, T>::end()
    {
        return indices.end();
    }

    template<class Container, class T>
    typename TypeMapBase<Container, T>::const_iterator TypeMapBase<Container, T>::end() const
    {
        return indices.end();
    }

    template<class Container, class T>
    typename TypeMapBase<Container, T>::size_type TypeMapBase<Container, T>::size() const
    {
        return indices.size();
    }

    template<class Container, class T>
    bool TypeMapBase<Container, T>::empty() const
    {
        return indices.empty();
    }

    template<class T>
    using TypeMap = TypeMapBase<std::unordered_map<std::type_index, T>, T>;

    template<class T>
    using OrderedTypeMap = TypeMapBase<std::map<std::type_index, T>, T>;
}