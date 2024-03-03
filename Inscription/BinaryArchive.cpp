#include "BinaryArchive.h"

#include "OutputBinaryArchive.h"
#include "InputBinaryArchive.h"

namespace Inscription
{
    BinaryArchive::~BinaryArchive()
    {}

    bool BinaryArchive::TrackObjects(bool set)
    {
        auto ret = objectTracker.IsActive();
        objectTracker.SetActive(set);
        return ret;
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

    const BinaryArchive::Signature& BinaryArchive::GetSignature() const
    {
        return signature;
    }

    Version BinaryArchive::GetVersion() const
    {
        return version;
    }

    void BinaryArchive::MovePositionToStart()
    {
        objectTracker.Clear();
        SeekStream(postHeaderPosition);
    }

    BinaryArchive::BinaryArchive(
        Direction direction,
        const Signature& signature,
        Version version) :

        direction(direction), signature(signature), version(version),
        postHeaderPosition(0)
    {}

    BinaryArchive::BinaryArchive(
        Direction direction,
        const Signature& signature,
        Version version,
        TypeRegistrationContext typeRegistrationContext) :

        direction(direction), signature(signature), version(version),
        typeRegistrationContext(typeRegistrationContext),
        postHeaderPosition(0)
    {
        typeRegistrationContext.PushAll(*this);
    }

    BinaryArchive::BinaryArchive(BinaryArchive&& arg) :
        Archive(std::move(arg)),
        direction(arg.direction), signature(std::move(arg.signature)), version(std::move(arg.version)),
        objectTracker(std::move(arg.objectTracker)), typeTracker(std::move(arg.typeTracker)),
        polymorphicManager(std::move(arg.polymorphicManager)),
        typeRegistrationContext(std::move(arg.typeRegistrationContext)),
        postHeaderPosition(std::move(arg.postHeaderPosition))
    {}

    BinaryArchive& BinaryArchive::operator=(BinaryArchive&& arg)
    {
        Archive::operator=(std::move(arg));
        signature = std::move(arg.signature);
        version = std::move(arg.version);
        objectTracker = std::move(arg.objectTracker);
        typeTracker = std::move(arg.typeTracker);
        polymorphicManager = std::move(arg.polymorphicManager);
        typeRegistrationContext = std::move(arg.typeRegistrationContext);
        postHeaderPosition = std::move(arg.postHeaderPosition);
        return *this;
    }
}