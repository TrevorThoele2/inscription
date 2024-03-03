#pragma once

#include "Archive.h"

#include "ObjectTracker.h"
#include "TypeTracker.h"
#include "PolymorphicManager.h"
#include "TypeRegistrationContext.h"
#include "ObjectTrackingTraits.h"

#include "Scribe.h"
#include "TableData.h"

#include "Direction.h"
#include "Endian.h"
#include "Const.h"
#include "Buffer.h"
#include "VerifyNonConst.h"

namespace Inscription
{
    class OutputBinaryArchive;
    class InputBinaryArchive;

    class BinaryArchive : public Archive
    {
    public:
        using Signature = std::string;
        using StreamPosition = unsigned long long;
        using TypeRegistrationContext = TypeRegistrationContext<BinaryArchive>;
    public:
        virtual ~BinaryArchive() = 0;
    public:
        template<class T>
        BinaryArchive& operator()(T& object);
        template<class T>
        BinaryArchive& operator()(T*& object);
    public:
        template<class T>
        Optional<TrackingID> AttemptTrackObject(T* arg);
        template<class T>
        void AttemptReplaceTrackedObject(T& here, T& newObject);
        void AttemptReplaceTrackedObject(void* here, void *newObject);
        bool TrackObjects(bool set = true);

        void TrackSavedConstruction(TrackingID trackingID);

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
        const Signature& ClientSignature() const;
        Version ClientVersion() const;
        Version InscriptionVersion() const;

        void MovePositionToStart();

        virtual void SeekStream(StreamPosition position) = 0;
        virtual StreamPosition TellStream() = 0;
    public:
        template<class T>
        void RegisterType();
    protected:
        Signature clientSignature;
        Version clientVersion;
        Version inscriptionVersion;
        StreamPosition postHeaderPosition;
    protected:
        BinaryArchive(
            Direction direction,
            const Signature& clientSignature,
            Version clientVersion,
            Version inscriptionVersion);
        BinaryArchive(
            Direction direction,
            const Signature& clientSignature,
            Version clientVersion,
            Version inscriptionVersion,
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
        KnownScribe<typename RemoveConstTrait<T>::type> scribe;
        scribe.Scriven(RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    BinaryArchive& BinaryArchive::operator()(T*& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type*> scribe;
        scribe.Scriven(RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    Optional<TrackingID> BinaryArchive::AttemptTrackObject(T* arg)
    {
        if (!ObjectTrackingTraits<T, BinaryArchive>::shouldTrack)
            return Optional<TrackingID>();

        return objectTracker.Add(arg);
    }

    template<class T>
    void BinaryArchive::AttemptReplaceTrackedObject(T& here, T& newObject)
    {
        if (!ObjectTrackingTraits<T, BinaryArchive>::shouldTrack)
            return;

        objectTracker.ReplaceObject(&here, &newObject);
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
        polymorphicManager.Register<T>(*this);
    }

    template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int>>
    void BinaryArchive::RegisterTypeImpl()
    {}

    template<class T>
    using BinaryScribe = Scribe<T, BinaryArchive>;
    template<class Object>
    using BinaryTableData = TableData<Object, BinaryArchive>;
}