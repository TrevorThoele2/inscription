#pragma once

namespace Inscription
{
    class Archive
    {
    public:
        Archive(const Archive& arg) = delete;
        Archive& operator=(const Archive& arg) = delete;

        virtual ~Archive() = 0;
    protected:
        Archive() = default;
        Archive(Archive&& arg) noexcept;
        Archive& operator=(Archive&& arg) noexcept;
    };
}