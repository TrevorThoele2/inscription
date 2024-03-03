#pragma once

#include <functional>

#include "TableData.h"

#include "Type.h"

namespace Inscription
{
    template<class Object, class Archive>
    class AutoTableDataEntry
    {
    public:
        using ObjectT = Object;
        using ArchiveT = Archive;

        using DataT = TableData<ObjectT, ArchiveT>;
    public:
        template<class T>
        static AutoTableDataEntry Auto(T ObjectT::*objectMember, T DataT::*dataMember);
        template<class BaseT>
        static AutoTableDataEntry Base(Type<BaseT>);

        AutoTableDataEntry(const AutoTableDataEntry& arg);
        AutoTableDataEntry(AutoTableDataEntry&& arg);
        
        AutoTableDataEntry& operator=(const AutoTableDataEntry& arg);
        AutoTableDataEntry& operator=(AutoTableDataEntry&& arg);

        void Scriven(DataT& data, ArchiveT& archive);

        void PullFromObject(DataT& data, ObjectT& object);
        void PushToObject(DataT& data, ObjectT& object);
    private:
        template<class T>
        AutoTableDataEntry(T ObjectT::*objectMember, T DataT::*dataMember);
        template<class BaseT>
        AutoTableDataEntry(Type<BaseT>);
    private:
        class Implementation
        {
        public:
            virtual ~Implementation() = 0;

            virtual Implementation* Clone() const = 0;

            virtual void Scriven(DataT& data, ArchiveT& archive) = 0;

            virtual void PullFromObject(DataT& data, ObjectT& object) = 0;
            virtual void PushToObject(DataT& data, ObjectT& object) = 0;
        };

        template<class T>
        class AutoImplementation : public Implementation
        {
        public:
            AutoImplementation(T ObjectT::*objectMember, T DataT::*dataMember);

            AutoImplementation* Clone() const override;

            void Scriven(DataT& data, ArchiveT& archive) override;

            void PullFromObject(DataT& data, ObjectT& object) override;
            void PushToObject(DataT& data, ObjectT& object) override;
        private:
            T ObjectT::*objectMember;
            T DataT::*dataMember;
        };

        template<class BaseT>
        class BaseImplementation : public Implementation
        {
        public:
            BaseImplementation* Clone() const override;

            void Scriven(DataT& data, ArchiveT& archive) override;

            void PullFromObject(DataT& data, ObjectT& object) override;
            void PushToObject(DataT& data, ObjectT& object) override;
        private:
            using BaseTable = Table<BaseT, ArchiveT>;
            BaseTable baseTable;
        };

        using ImplementationPtr = std::unique_ptr<Implementation>;
        ImplementationPtr implementation;
    };

    template<class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Object, Archive> AutoTableDataEntry<Object, Archive>::Auto(
        T ObjectT::*objectMember, T DataT::*dataMember)
    {
        return AutoTableDataEntry(objectMember, dataMember);
    }

    template<class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Object, Archive> AutoTableDataEntry<Object, Archive>::Base(
        Type<BaseT> type)
    {
        return AutoTableDataEntry(type);
    }

    template<class Object, class Archive>
    AutoTableDataEntry<Object, Archive>::AutoTableDataEntry(const AutoTableDataEntry& arg) :
        implementation(arg.implementation->Clone())
    {}

    template<class Object, class Archive>
    AutoTableDataEntry<Object, Archive>::AutoTableDataEntry(AutoTableDataEntry&& arg) :
        implementation(std::move(arg.implementation))
    {}

    template<class Object, class Archive>
    AutoTableDataEntry<Object, Archive>& AutoTableDataEntry<Object, Archive>::operator=(const AutoTableDataEntry& arg)
    {
        implementation.reset(arg.implementation->Clone());
        return *this;
    }

    template<class Object, class Archive>
    AutoTableDataEntry<Object, Archive>& AutoTableDataEntry<Object, Archive>::operator=(AutoTableDataEntry&& arg)
    {
        implementation = std::move(arg.implementation);
        return *this;
    }

    template<class Object, class Archive>
    void AutoTableDataEntry<Object, Archive>::Scriven(DataT& data, ArchiveT& archive)
    {
        implementation->Scriven(data, archive);
    }

    template<class Object, class Archive>
    void AutoTableDataEntry<Object, Archive>::PullFromObject(DataT& data, ObjectT& object)
    {
        implementation->PullFromObject(data, object);
    }

    template<class Object, class Archive>
    void AutoTableDataEntry<Object, Archive>::PushToObject(DataT& data, ObjectT& object)
    {
        implementation->PushToObject(data, object);
    }

    template<class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Object, Archive>::AutoTableDataEntry(T ObjectT::*objectMember, T DataT::*dataMember) :
        implementation(new AutoImplementation<T>(objectMember, dataMember))
    {}

    template<class Object, class Archive>
    template<class BaseT>
    AutoTableDataEntry<Object, Archive>::AutoTableDataEntry(Type<BaseT>) :
        implementation(new BaseImplementation<BaseT>())
    {}

    template<class Object, class Archive>
    AutoTableDataEntry<Object, Archive>::Implementation::~Implementation()
    {}

    template<class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Object, Archive>::AutoImplementation<T>::AutoImplementation(
        T ObjectT::*objectMember, T DataT::*dataMember) :

        objectMember(objectMember), dataMember(dataMember)
    {}

    template<class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Object, Archive>::AutoImplementation<T>*
        AutoTableDataEntry<Object, Archive>::AutoImplementation<T>::Clone() const
    {
        return new AutoImplementation(*this);
    }

    template<class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Object, Archive>::AutoImplementation<T>::Scriven(DataT& data, ArchiveT& archive)
    {
        archive(data.*dataMember);
    }

    template<class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Object, Archive>::AutoImplementation<T>::PullFromObject(DataT& data, ObjectT& object)
    {
        (data.*dataMember) = (object.*objectMember);
    }

    template<class Object, class Archive>
    template<class T>
    void AutoTableDataEntry<Object, Archive>::AutoImplementation<T>::PushToObject(DataT& data, ObjectT& object)
    {
        (object.*objectMember) = (data.*dataMember);
    }

    template<class Object, class Archive>
    template<class T>
    AutoTableDataEntry<Object, Archive>::BaseImplementation<T>*
        AutoTableDataEntry<Object, Archive>::BaseImplementation<T>::Clone() const
    {
        return new BaseImplementation(*this);
    }

    template<class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Object, Archive>::BaseImplementation<BaseT>::Scriven(DataT& data, ArchiveT& archive)
    {
        baseTable.Scriven(data, archive);
    }

    template<class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Object, Archive>::BaseImplementation<BaseT>::PullFromObject(DataT& data, ObjectT& object)
    {
        baseTable.PullFromObject(data, object);
    }

    template<class Object, class Archive>
    template<class BaseT>
    void AutoTableDataEntry<Object, Archive>::BaseImplementation<BaseT>::PushToObject(DataT& data, ObjectT& object)
    {
        baseTable.PushToObject(data, object);
    }
}