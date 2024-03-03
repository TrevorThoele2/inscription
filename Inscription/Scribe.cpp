
#include "Scribe.h"

namespace inscription
{
    ScribeDirectionException::ScribeDirectionException() : Exception("Attempted operation in the opposite direction of the scribe. (Saving while scribe is a loader and vice versa)")
    {}

    ScribeExistenceException::ScribeExistenceException() : Exception("This object was assumed to exist but it does not. Make sure it saved correctly or use the regular serialization functions.")
    {}

    Scribe::~Scribe()
    {}

    void Scribe::WriteBuffer(const Buffer &write)
    {
        WriteImpl(write);
    }

    void Scribe::ReadBuffer(Buffer &read)
    {
        ReadImpl(read);
    }

    Buffer Scribe::ReadBuffer()
    {
        Buffer buffer;
        ReadImpl(buffer);
        return buffer;
    }

    bool Scribe::IsOutput() const
    {
        return direction == Direction::OUTPUT;
    }

    bool Scribe::IsInput() const
    {
        return direction == Direction::INPUT;
    }

    bool Scribe::TrackObjects(bool set)
    {
        auto ret = trackers.IsActive();
        trackers.SetActive(set);
        return ret;
    }

    void Scribe::StartTrackingSection()
    {
        trackers.StartSection();
    }

    void Scribe::StopTrackingSection(bool clear)
    {
        if (clear)
            trackers.ClearSection();

        trackers.StopSection();
    }

    void Scribe::ClearTrackingSection()
    {
        trackers.ClearSection();
    }

    void Scribe::CopyTrackersTo(Scribe &scribe) const
    {
        scribe.trackers = trackers;
    }

    void Scribe::MoveTrackersTo(Scribe &scribe)
    {
        scribe.trackers = std::move(trackers);
    }

    void Scribe::Reset()
    {
        trackers.Clear();
        SeekStream(GetPostHeaderPosition());
    }

    Scribe::Scribe(Direction direction) : direction(direction), polyManager(direction)
    {
        polyManager.Fill(*this);
    }

    Scribe::Scribe(Scribe &&arg) : polyManager(std::move(arg.polyManager)), direction(arg.direction), trackers(std::move(arg.trackers))
    {}

    Scribe& Scribe::operator=(Scribe &&arg)
    {
        polyManager = std::move(arg.polyManager);
        direction = arg.direction;
        trackers = std::move(arg.trackers);
        return *this;
    }

    void Scribe::OnExecuteOutput() const
    {
        if (direction == Direction::INPUT)
            throw ScribeDirectionException();
    }

    void Scribe::OnExecuteInput() const
    {
        if (direction == Direction::OUTPUT)
            throw ScribeDirectionException();
    }
}