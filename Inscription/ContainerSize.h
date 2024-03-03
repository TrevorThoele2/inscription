
#pragma once

#include <cstddef>

#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
    #include <cstdint>
#endif

namespace Inscription
{
    class Scribe;
    class ContainerSize
    {
    public:
#ifndef INSCRIPTION_CONTAINER_SIZE_VALUE_T
        typedef std::size_t ValueT;
#else
        typedef INSCRIPTION_CONTAINER_SIZE_VALUE_T ValueT;
#endif
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
}