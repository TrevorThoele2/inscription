
#include "ContainerSize.h"
#include "Scribe.h"

namespace inscription
{
    void ContainerSize::Serialize(Scribe &scribe)
    {
        scribe(value);
    }

    ContainerSize::ContainerSize() : value(0)
    {}

    ContainerSize::ContainerSize(ValueT value) : value(value)
    {}

    bool ContainerSize::operator==(const ContainerSize &arg) const
    {
        return value == arg.value;
    }

    bool ContainerSize::operator!=(const ContainerSize &arg) const
    {
        return !(*this == arg);
    }

    bool ContainerSize::operator>(const ContainerSize &arg) const
    {
        return value > arg.value;
    }

    bool ContainerSize::operator>=(const ContainerSize &arg) const
    {
        return (*this > arg) || (*this == arg);
    }

    bool ContainerSize::operator<(const ContainerSize &arg) const
    {
        return value < arg.value;
    }

    bool ContainerSize::operator<=(const ContainerSize &arg) const
    {
        return (*this < arg) || (*this == arg);
    }

    ContainerSize& ContainerSize::operator--()
    {
        value--;
        return *this;
    }

    ContainerSize ContainerSize::operator--(int)
    {
        auto prev = *this;
        value--;
        return prev;
    }

    ContainerSize::ValueT ContainerSize::Get() const
    {
        return value;
    }
}