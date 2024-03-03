
#pragma once

#include <typeindex>
#include <unordered_map>
#include <map>

namespace inscription
{
    template<class Cont, class T>
    class TypeTableBase
    {
    private:
        typedef Cont Indexes;
    public:
        typedef typename Indexes::iterator iterator;
        typedef typename Indexes::const_iterator const_iterator;
        typedef typename Indexes::size_type size_type;
    private:
        Indexes indexes;
    public:
        TypeTableBase() = default;
        TypeTableBase(const TypeTableBase &arg) = default;
        TypeTableBase(TypeTableBase &&arg);
        TypeTableBase& operator=(const TypeTableBase &arg) = default;
        TypeTableBase& operator=(TypeTableBase &&arg);
        std::pair<iterator, bool> Add(T &obj);
        std::pair<iterator, bool> Add(T &&obj);
        std::pair<iterator, bool> Add(T &obj, const std::type_info &info);
        std::pair<iterator, bool> Add(T &&obj, const std::type_info &info);
        std::pair<iterator, bool> Add(T &obj, const std::type_index &info);
        std::pair<iterator, bool> Add(T &&obj, const std::type_index &info);

        T* Find(const std::type_info &info);
        const T* Find(const std::type_info &info) const;
        T* Find(const std::type_index &index);
        const T* Find(const std::type_index &index) const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        size_type size() const;
        bool empty() const;
    };
    
    template<class Cont, class T>
    TypeTableBase<Cont, T>::TypeTableBase(TypeTableBase &&arg) : indexes(std::move(arg.indexes))
    {}

    template<class Cont, class T>
    TypeTableBase<Cont, T>& TypeTableBase<Cont, T>::operator=(TypeTableBase &&arg)
    {
        indexes = std::move(arg.indexes);
        return *this;
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &obj)
    {
        return indexes.emplace(std::type_index(typeid(obj)), obj);
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &&obj)
    {
        return indexes.emplace(std::type_index(typeid(obj)), std::move(obj));
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &obj, const std::type_info &info)
    {
        return indexes.emplace(std::type_index(info), obj);
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &&obj, const std::type_info &info)
    {
        return indexes.emplace(std::type_index(info), std::move(obj));
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &obj, const std::type_index &info)
    {
        return indexes.emplace(info, obj);
    }

    template<class Cont, class T>
    std::pair<typename TypeTableBase<Cont, T>::iterator, bool> TypeTableBase<Cont, T>::Add(T &&obj, const std::type_index &info)
    {
        return indexes.emplace(info, std::move(obj));
    }

    template<class Cont, class T>
    T* TypeTableBase<Cont, T>::Find(const std::type_info &info)
    {
        return Find(std::type_index(info));
    }

    template<class Cont, class T>
    const T* TypeTableBase<Cont, T>::Find(const std::type_info &info) const
    {
        return Find(std::type_index(info));
    }

    template<class Cont, class T>
    T* TypeTableBase<Cont, T>::Find(const std::type_index &index)
    {
        auto possibleFound = indexes.find(index);
        if(possibleFound != indexes.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Cont, class T>
    const T* TypeTableBase<Cont, T>::Find(const std::type_index &index) const
    {
        auto possibleFound = indexes.find(index);
        if (possibleFound != indexes.end())
            return &possibleFound->second;

        return nullptr;
    }

    template<class Cont, class T>
    typename TypeTableBase<Cont, T>::iterator TypeTableBase<Cont, T>::begin()
    {
        return indexes.begin();
    }

    template<class Cont, class T>
    typename TypeTableBase<Cont, T>::const_iterator TypeTableBase<Cont, T>::begin() const
    {
        return indexes.begin();
    }

    template<class Cont, class T>
    typename TypeTableBase<Cont, T>::iterator TypeTableBase<Cont, T>::end()
    {
        return indexes.end();
    }

    template<class Cont, class T>
    typename TypeTableBase<Cont, T>::const_iterator TypeTableBase<Cont, T>::end() const
    {
        return indexes.end();
    }

    template<class Cont, class T>
    typename TypeTableBase<Cont, T>::size_type TypeTableBase<Cont, T>::size() const
    {
        return indexes.size();
    }

    template<class Cont, class T>
    bool TypeTableBase<Cont, T>::empty() const
    {
        return indexes.empty();
    }

    template<class T>
    using TypeTable = TypeTableBase<std::unordered_map<std::type_index, T>, T>;

    template<class T>
    using OrderedTypeTable = TypeTableBase<std::map<std::type_index, T>, T>;
}