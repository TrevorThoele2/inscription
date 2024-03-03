#include "BinaryScribe.h"

#include "OutputBinaryScribe.h"
#include "InputBinaryScribe.h"

namespace Inscription
{
    BinaryScribe::~BinaryScribe()
    {}

    bool BinaryScribe::TrackObjects(bool set)
    {
        auto ret = trackers.IsActive();
        trackers.SetActive(set);
        return ret;
    }

    void BinaryScribe::StartTrackingSection()
    {
        trackers.StartSection();
    }

    void BinaryScribe::StopTrackingSection(bool clear)
    {
        if (clear)
            trackers.ClearSection();

        trackers.StopSection();
    }

    void BinaryScribe::ClearTrackingSection()
    {
        trackers.ClearSection();
    }

    void BinaryScribe::CopyTrackersTo(BinaryScribe& scribe) const
    {
        scribe.trackers = trackers;
    }

    void BinaryScribe::MoveTrackersTo(BinaryScribe& scribe)
    {
        scribe.trackers = std::move(trackers);
    }

    bool BinaryScribe::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool BinaryScribe::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    OutputBinaryScribe* BinaryScribe::AsOutput()
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<OutputBinaryScribe*>(this);
    }

    InputBinaryScribe* BinaryScribe::AsInput()
    {
        if (!IsInput())
            return nullptr;

        return static_cast<InputBinaryScribe*>(this);
    }

    const OutputBinaryScribe* BinaryScribe::AsOutput() const
    {
        if (!IsOutput())
            return nullptr;

        return static_cast<const OutputBinaryScribe*>(this);
    }

    const InputBinaryScribe* BinaryScribe::AsInput() const
    {
        if (!IsInput())
            return nullptr;

        return static_cast<const InputBinaryScribe*>(this);
    }

    const BinaryScribe::Signature& BinaryScribe::GetSignature() const
    {
        return signature;
    }

    Version BinaryScribe::GetVersion() const
    {
        return version;
    }

    void BinaryScribe::MovePositionToStart()
    {
        trackers.Clear();
        SeekStream(postHeaderPosition);
    }

    BinaryScribe::BinaryScribe(Direction direction, const Signature& signature, Version version) :
        direction(direction), signature(signature), version(version),
        pointerManager(direction, registeredTypes),
        postHeaderPosition(0)
    {
        registeredTypes.CopyRegisteredTo(trackers);
        registeredTypes.CopyRegisteredTo(pointerManager, *this);
    }

    BinaryScribe::BinaryScribe(BinaryScribe&& arg) :
        Scribe(std::move(arg)),
        direction(arg.direction), signature(std::move(arg.signature)), version(std::move(arg.version)),
        pointerManager(arg.direction, registeredTypes),
        postHeaderPosition(std::move(arg.postHeaderPosition))
    {
        registeredTypes.CopyRegisteredTo(trackers);
        registeredTypes.CopyRegisteredTo(pointerManager, *this);
    }

    BinaryScribe& BinaryScribe::operator=(BinaryScribe&& arg)
    {
        Scribe::operator=(std::move(arg));
        signature = std::move(arg.signature);
        version = std::move(arg.version);
        pointerManager = std::move(arg.pointerManager);
        postHeaderPosition = std::move(arg.postHeaderPosition);
        return *this;
    }

    RegisteredTypes<BinaryScribe> BinaryScribe::registeredTypes;
}