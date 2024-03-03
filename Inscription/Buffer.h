#pragma once

namespace Inscription
{
    // The buffer has ownership over the value while it is within this
    struct Buffer
    {
        using SizeT = size_t;
        using T = char*;

        SizeT size;
        T value;

        Buffer();
        // This will allocate the underlying buffer to this size
        explicit Buffer(SizeT size);
        // Takes ownership of the value
        Buffer(SizeT size, T value);
        Buffer(const Buffer& arg);
        Buffer(Buffer&& arg) noexcept;

        ~Buffer();

        Buffer& operator=(const Buffer& arg);
        Buffer& operator=(Buffer&& arg) noexcept;
        bool operator==(const Buffer& arg) const;
        bool operator!=(const Buffer& arg) const;

        explicit operator bool() const;

        // Takes ownership of the value
        void Set(SizeT size, T value);
        [[nodiscard]] bool IsOccupied() const;
    };
}