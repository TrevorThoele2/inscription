#pragma once

namespace inscription
{
    // The buffer has ownership over the value while it is within this
    struct Buffer
    {
        typedef size_t SizeT;
        typedef char* T;

        SizeT size;
        T value;

        Buffer();
        // This will allocate the underlying buffer to this size
        Buffer(SizeT size);
        // Takes ownership of the value
        Buffer(SizeT size, T value);
        Buffer(const Buffer &arg);
        Buffer(Buffer &&arg);
        ~Buffer();
        Buffer& operator=(const Buffer &arg);
        Buffer& operator=(Buffer &&arg);
        bool operator==(const Buffer &arg) const;
        bool operator!=(const Buffer &arg) const;
        explicit operator bool() const;

        // Takes ownership of the value
        void Set(SizeT size, T value);
        SizeT GetSize() const;
        T GetValue() const;
        bool IsOccupied() const;
    };
}