#pragma once

#include "Archive.h"

#include "ObjectTracker.h"
#include "TypeTracker.h"
#include "PolymorphicManager.h"
#include "TypeRegistrationContext.h"

#include "Scribe.h"

#include "Direction.h"
#include "Endian.h"
#include "Const.h"
#include "Buffer.h"
#include "VerifyNonConst.h"

#include "RegisteredTypeNotFound.h"

namespace Inscription
{
    class OutputBinaryArchive;
    class InputBinaryArchive;

    class BinaryArchive : public Archive
    {
    public:
        typedef std::string Signature;

        typedef unsigned long long StreamPosition;
        
        typedef TypeRegistrationContext<BinaryArchive> TypeRegistrationContext;
    public:
        virtual ~BinaryArchive() = 0;
    public:
        template<class T>
        BinaryArchive& operator()(T& object);
        template<class T>
        BinaryArchive& operator()(T*& object);
    public:
        template<class T>
        void TrackObject(T* arg);
        template<class T>
        void ReplaceTrackedObject(T& here, T& newObj);
        bool TrackObjects(bool set = true);

        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        void StartTrackingSection();
        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        // This does NOT destroy the section entries if the clear variable is false
        void StopTrackingSection(bool clear = false);
        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        // This WILL destroy the section entries, and keep the section active
        void ClearTrackingSection();

        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        void CopyTrackersTo(BinaryArchive& target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this archive will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(BinaryArchive& target);
    public:
        bool IsOutput() const;
        bool IsInput() const;

        OutputBinaryArchive* AsOutput();
        InputBinaryArchive* AsInput();
        const OutputBinaryArchive* AsOutput() const;
        const InputBinaryArchive* AsInput() const;
    public:
        const Signature& GetSignature() const;
        Version GetVersion() const;

        void MovePositionToStart();

        virtual void SeekStream(StreamPosition position) = 0;
        virtual StreamPosition TellStream() = 0;
    public:
        template<class T>
        void RegisterType();
    protected:
        Signature signature;
        Version version;
        StreamPosition postHeaderPosition;
    protected:
        BinaryArchive(
            Direction direction,
            const Signature& signature,
            Version version);
        BinaryArchive(
            Direction direction,
            const Signature& signature,
            Version version,
            TypeRegistrationContext typeRegistrationContext);
        BinaryArchive(BinaryArchive&& arg);

        BinaryArchive& operator=(BinaryArchive&& arg);
    private:
        template<class T>
        using KnownScribe = Scribe<T, BinaryArchive>;
    private:
        const Direction direction;
    private:
        ObjectTracker objectTracker;
        TypeTracker typeTracker;

        PolymorphicManager<BinaryArchive> polymorphicManager;
    private:
        BinaryArchive(const BinaryArchive& arg) = delete;
        BinaryArchive& operator=(const BinaryArchive& arg) = delete;
    private:
        TypeRegistrationContext typeRegistrationContext;

        template<class T>
        inline void EnsureTypeRegistered() const
        {
            if (!typeRegistrationContext.HasStored<T>())
                throw RegisteredTypeNotFound(typeid(T));
        }

        template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
        template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
    private:
        template<class Archive>
        friend class PolymorphicManager;
        template<class Object, class Archive>
        friend class PointerScribe;
    };

    template<class T>
    BinaryArchive& BinaryArchive::operator()(T& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type>::Scriven(RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    BinaryArchive& BinaryArchive::operator()(T*& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type*>::Scriven(RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    void BinaryArchive::TrackObject(T* arg)
    {
        objectTracker.Add(arg);
    }

    template<class T>
    void BinaryArchive::ReplaceTrackedObject(T& here, T& newObj)
    {
        objectTracker.ReplaceObject(here, newObj);
    }

    template<class T>
    void BinaryArchive::RegisterType()
    {
        static_assert(std::is_class_v<T>, "A registered type must be a class type.");

        typeRegistrationContext.RegisterType<T>();
        RegisterTypeImpl<T>();
    }

    template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int>>
    void BinaryArchive::RegisterTypeImpl()
    {
        objectTracker.Register<T>();
        polymorphicManager.Register<T>(*this);
    }

    template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int>>
    void BinaryArchive::RegisterTypeImpl()
    {
        objectTracker.Register<T>();
    }
}