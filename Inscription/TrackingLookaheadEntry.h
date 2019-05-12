#pragma once

namespace Inscription
{
    class TrackingLookaheadEntry
    {
    public:
        void** position;
    public:
        TrackingLookaheadEntry(void** position);
    };
}