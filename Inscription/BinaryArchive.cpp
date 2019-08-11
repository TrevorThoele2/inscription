#include "BinaryArchive.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    BinaryArchive::~BinaryArchive()
    {}

    void BinaryArchive::AttemptReplaceTrackedObject(void* here, void *newObject)
    {
        objectTracker.ReplaceObject(here, newObject);
    }

    bool BinaryArchive::TrackObjects(bool set)
    {
        auto ret = objectTracker.IsActive();
        objectTracker.Activate(set);
        return ret;
    }

    void BinaryArchive::TrackSavedConstruction(TrackingID trackingID)
    {
        objectTracker.SignalSavedConstruction(trackingID);
    }

    void BinaryArchive::CopyTrackersTo(BinaryArchive& scribe) const
    {
        scribe.objectTracker = objectTracker;
    }

    void BinaryArchive::MoveTrackersTo(BinaryArchive& scribe)
    {
        scribe.objectTracker = std::move(objectTracker);
    }

    bool BinaryArchive::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool BinaryArchive::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    OutputBinaryArchive* BinaryArchive::AsOutput()
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<OutputBinaryArchive*>(this);
    }

    InputBinaryArchive* BinaryArchive::AsInput()
    {
        if (!IsInput())
            return nullptr;

        return static_cast<InputBinaryArchive*>(this);
    }

    const OutputBinaryArchive* BinaryArchive::AsOutput() const
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<const OutputBinaryArchive*>(this);
    }

    const InputBinaryArchive* BinaryArchive::AsInput() const
    {
        if (!IsInput())
            return nullptr;

        return static_cast<const InputBinaryArchive*>(this);
    }

    const BinaryArchive::Signature& BinaryArchive::ClientSignature() const
    {
        return clientSignature;
    }

    Version BinaryArchive::ClientVersion() const
    {
        return clientVersion;
    }

    Version BinaryArchive::InscriptionVersion() const
    {
        return inscriptionVersion;
    }

    void BinaryArchive::MovePositionToStart()
    {
        objectTracker.Clear();
        SeekStream(postHeaderPosition);
    }

    BinaryArchive::BinaryArchive(
        Direction direction,
        const Signature& clientSignature,
        Version clientVersion,
        Version inscriptionVersion) :

        direction(direction), clientSignature(clientSignature), clientVersion(clientVersion),
        inscriptionVersion(inscriptionVersion), postHeaderPosition(0)
    {}

    BinaryArchive::BinaryArchive(
        Direction direction,
        const Signature& clientSignature,
        Version clientVersion,
        Version inscriptionVersion,
        TypeRegistrationContext typeRegistrationContext) :

        direction(direction), clientSignature(clientSignature), clientVersion(clientVersion),
        inscriptionVersion(inscriptionVersion), typeRegistrationContext(typeRegistrationContext), postHeaderPosition(0)
    {
        typeRegistrationContext.PushAll(*this);
    }

    BinaryArchive::BinaryArchive(BinaryArchive&& arg) :
        Archive(std::move(arg)),
        direction(arg.direction), clientSignature(std::move(arg.clientSignature)),
        clientVersion(std::move(arg.clientVersion)), objectTracker(std::move(arg.objectTracker)),
        typeTracker(std::move(arg.typeTracker)), polymorphicManager(std::move(arg.polymorphicManager)),
        typeRegistrationContext(std::move(arg.typeRegistrationContext)),
        postHeaderPosition(std::move(arg.postHeaderPosition))
    {}

    BinaryArchive& BinaryArchive::operator=(BinaryArchive&& arg)
    {
        Archive::operator=(std::move(arg));
        clientSignature = std::move(arg.clientSignature);
        clientVersion = std::move(arg.clientVersion);
        objectTracker = std::move(arg.objectTracker);
        typeTracker = std::move(arg.typeTracker);
        polymorphicManager = std::move(arg.polymorphicManager);
        typeRegistrationContext = std::move(arg.typeRegistrationContext);
        postHeaderPosition = std::move(arg.postHeaderPosition);
        return *this;
    }
}