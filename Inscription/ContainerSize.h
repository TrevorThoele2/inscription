
#pragma once

#include <cstddef>
#include "TrackingTraits.h"

namespace inscription
{
    class Scribe;
    class ContainerSize
    {
    public:
        typedef std::size_t ValueT;
    private:
        void Serialize(Scribe &scribe);
        friend class Access;
    private:
        ValueT value;
    public:
        ContainerSize();
        ContainerSize(ValueT value);
        bool operator==(const ContainerSize &arg) const;
        bool operator!=(const ContainerSize &arg) const;
        bool operator>(const ContainerSize &arg) const;
        bool operator>=(const ContainerSize &arg) const;
        bool operator<(const ContainerSize &arg) const;
        bool operator<=(const ContainerSize &arg) const;
        ContainerSize& operator--();
        ContainerSize operator--(int);
        ValueT Get() const;
    };

    template<>
    struct TrackPointer<ContainerSize>
    { static constexpr TrackEnum value = TRACK_NEVER; };
}