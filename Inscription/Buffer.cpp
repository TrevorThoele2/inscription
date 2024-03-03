#include "Buffer.h"

#include <utility>

namespace Inscription
{
    Buffer::Buffer() : size(0), value(nullptr)
    {}

    Buffer::Buffer(SizeT size) : size(size), value(new char[size])
    {}

    Buffer::Buffer(SizeT size, T value) : size(size), value(value)
    {}

    Buffer::Buffer(const Buffer& arg) : size(arg.size), value(new char[arg.size])
    {
        memcpy(value, arg.value, size);
    }

    Buffer::Buffer(Buffer&& arg) : size(std::move(arg.size)), value(std::move(arg.value))
    {
        arg.size = 0;
        arg.value = nullptr;
    }

    Buffer::~Buffer()
    {
        if (value)
            delete[] value;
    }

    Buffer& Buffer::operator=(const Buffer& arg)
    {
        size = arg.size;
        value = new char[size];
        memcpy(value, arg.value, size);
        return *this;
    }

    Buffer& Buffer::operator=(Buffer&& arg)
    {
        size = std::move(arg.size);
        value = std::move(arg.value);
        arg.size = 0;
        arg.value = nullptr;
        return *this;
    }

    bool Buffer::operator==(const Buffer& arg) const
    {
        return size == arg.size && value == arg.value;
    }

    bool Buffer::operator!=(const Buffer& arg) const
    {
        return !(*this == arg);
    }

    Buffer::operator bool() const
    {
        return IsOccupied();
    }

    void Buffer::Set(SizeT size, T value)
    {
        this->size = size;
        this->value = value;
    }

    Buffer::SizeT Buffer::GetSize() const
    {
        return size;
    }

    Buffer::T Buffer::GetValue() const
    {
        return value;
    }

    bool Buffer::IsOccupied() const
    {
        return value != nullptr;
    }
}