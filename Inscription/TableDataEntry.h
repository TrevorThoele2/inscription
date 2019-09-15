#pragma once

#include <array>

#include "ScribeBase.h"
#include "Access.h"
#include "ArrayScribe.h"

namespace Inscription
{
    template<class T>
    class TableDataEntry
    {
    public:
        TableDataEntry();
        TableDataEntry(const TableDataEntry& arg) = default;
        TableDataEntry(TableDataEntry&& arg) = default;
        explicit TableDataEntry(const T& arg);
        explicit TableDataEntry(T&& arg);

        ~TableDataEntry();

        TableDataEntry& operator=(const TableDataEntry& arg) = default;
        TableDataEntry& operator=(TableDataEntry&& arg) = default;
        TableDataEntry& operator=(const T& arg);
        TableDataEntry& operator=(T&& arg);

        operator T&();
        operator const T&() const;

        T& Get();
        const T& Get() const;

        TableDataEntry* operator&();
    private:
        using Wrapped = T;

        bool constructedBytes = false;
        using Bytes = std::array<char, sizeof(T)>;
        Bytes bytes;

        void Edit(const T& arg);
        void Edit(T&& arg);
        void Edit(Bytes&& bytes);
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    TableDataEntry<T>::TableDataEntry()
    {}

    template<class T>
    TableDataEntry<T>::TableDataEntry(const T& arg)
    {
        Edit(arg);
    }

    template<class T>
    TableDataEntry<T>::TableDataEntry(T&& arg)
    {
        Edit(std::move(arg));
    }

    template<class T>
    TableDataEntry<T>::~TableDataEntry()
    {
        if (constructedBytes)
            Get().~T();
    }

    template<class T>
    TableDataEntry<T>& TableDataEntry<T>::operator=(const T& arg)
    {
        Edit(arg);
        return *this;
    }

    template<class T>
    TableDataEntry<T>& TableDataEntry<T>::operator=(T&& arg)
    {
        Edit(std::move(arg));
        return *this;
    }

    template<class T>
    TableDataEntry<T>::operator T&()
    {
        return Get();
    }
    
    template<class T>
    TableDataEntry<T>::operator const T&() const
    {
        return Get();
    }

    template<class T>
    T& TableDataEntry<T>::Get()
    {
        return reinterpret_cast<T&>(*bytes.data());
    }

    template<class T>
    const T& TableDataEntry<T>::Get() const
    {
        return reinterpret_cast<const T&>(*bytes.data());
    }

    template<class T>
    auto TableDataEntry<T>::operator&() -> TableDataEntry*
    {
        return reinterpret_cast<T*>(bytes.data());
    }

    template<class T>
    void TableDataEntry<T>::Edit(const T& arg)
    {
        constructedBytes = true;
        new (reinterpret_cast<T*>(bytes.data())) T(arg);
    }

    template<class T>
    void TableDataEntry<T>::Edit(T&& arg)
    {
        constructedBytes = true;
        new (reinterpret_cast<T*>(bytes.data())) T(std::move(arg));
    }

    template<class T>
    void TableDataEntry<T>::Edit(Bytes&& bytes)
    {
        constructedBytes = true;
        bytes = std::move(bytes);
    }

    template<class T>
    class Scribe<TableDataEntry<T>, BinaryArchive> : public ScribeBase<TableDataEntry<T>, BinaryArchive>
    {
    private:
        using BaseT = ScribeBase<TableDataEntry<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.Get());
        }
    };
   
    template<class T, class Archive>
    struct ObjectTrackingTraits<TableDataEntry<T>, Archive>
    {
        static constexpr bool shouldTrack = false;
    };
}