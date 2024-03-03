#pragma once

#include "Version.h"

namespace Inscription
{
    class Archive
    {
    public:
        virtual ~Archive() = 0;
    protected:
        Archive();
        Archive(Archive&& arg);
        Archive& operator=(Archive&& arg);
    private:
        Archive(const Archive& arg) = delete;
        Archive& operator=(const Archive& arg) = delete;
    };
}