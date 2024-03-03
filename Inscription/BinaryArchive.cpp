#include "BinaryArchive.h"
#include <utility>

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    BinaryArchive::~BinaryArchive() = default;

    void BinaryArchive::AttemptReplaceTrackedObject(void* here, void *newObject)
    {
        objectTracker.ReplaceObject(here, newObject);
    }

    bool BinaryArchive::TrackObjects(bool set)
    {
        const auto isActive = objectTracker.IsActive();
        objectTracker.Activate(set);
        return isActive;
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
        return direction == Direction::Output;
    }

    bool BinaryArchive::IsInput() const
    {
        return direction == Direction::Input;
    }

    OutputBinaryArchive* BinaryArchive::AsOutput()
    {
        return dynamic_cast<OutputBinaryArchive*>(this);
    }

    InputBinaryArchive* BinaryArchive::AsInput()
    {
        return dynamic_cast<InputBinaryArchive*>(this);
    }

    const OutputBinaryArchive* BinaryArchive::AsOutput() const
    {
        return dynamic_cast<const OutputBinaryArchive*>(this);
    }

    const InputBinaryArchive* BinaryArchive::AsInput() const
    {
        return dynamic_cast<const InputBinaryArchive*>(this);
    }

    auto BinaryArchive::ClientSignature() const -> Signature
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
        TypeRegistrationContext typeRegistrationContext)
        :
        clientSignature(clientSignature), clientVersion(clientVersion), inscriptionVersion(inscriptionVersion),
        postHeaderPosition(0), direction(direction), typeRegistrationContext(typeRegistrationContext)
    {
        typeRegistrationContext.PushAll(*this);
    }

    BinaryArchive::BinaryArchive(BinaryArchive&& arg) noexcept :
        Archive(std::move(arg)),
        clientSignature(std::move(arg.clientSignature)), clientVersion(std::move(arg.clientVersion)),
        inscriptionVersion(std::move(arg.inscriptionVersion)),
        postHeaderPosition(std::move(arg.postHeaderPosition)), direction(arg.direction),
        objectTracker(std::move(arg.objectTracker)), typeTracker(std::move(arg.typeTracker)),
        polymorphicManager(std::move(arg.polymorphicManager)),
        typeRegistrationContext(std::move(arg.typeRegistrationContext))
    {}

    BinaryArchive& BinaryArchive::operator=(BinaryArchive&& arg) noexcept
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