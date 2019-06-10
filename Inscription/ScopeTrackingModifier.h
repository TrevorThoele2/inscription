#pragma once

namespace Inscription
{
    class BinaryArchive;

    class ScopeTrackingModifier
    {
    public:
        ScopeTrackingModifier(BinaryArchive& archive, bool shouldTrack);
        ~ScopeTrackingModifier();
    private:
        bool previousValue;
        BinaryArchive* archive;
    };
}