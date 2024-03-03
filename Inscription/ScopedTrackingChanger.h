#pragma once

namespace Inscription
{
    class BinaryScribe;

    class ScopedTrackingChanger
    {
    public:
        ScopedTrackingChanger(BinaryScribe& scribe, bool shouldTrack);
        ~ScopedTrackingChanger();
    private:
        bool previousValue;
        BinaryScribe* scribe;
    };
}