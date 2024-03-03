#include "BinaryScribe.h"

namespace Inscription
{
    BinaryScribe::~BinaryScribe()
    {}

    void BinaryScribe::WriteBuffer(const Buffer& write)
    {
        WriteImpl(write);
    }

    void BinaryScribe::ReadBuffer(Buffer& read)
    {
        ReadImpl(read);
    }

    Buffer BinaryScribe::ReadBuffer()
    {
        Buffer buffer;
        ReadBuffer(buffer);
        return buffer;
    }

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
        pointerManager(direction),
        postHeaderPosition(0)
    {}

    BinaryScribe::BinaryScribe(BinaryScribe&& arg) :
        Scribe(std::move(arg)),
        direction(arg.direction), signature(std::move(arg.signature)), version(std::move(arg.version)),
        pointerManager(arg.direction),
        postHeaderPosition(std::move(arg.postHeaderPosition))
    {}

    BinaryScribe& BinaryScribe::operator=(BinaryScribe&& arg)
    {
        Scribe::operator=(std::move(arg));
        signature = std::move(arg.signature);
        version = std::move(arg.version);
        pointerManager = std::move(arg.pointerManager);
        postHeaderPosition = std::move(arg.postHeaderPosition);
        return *this;
    }
}