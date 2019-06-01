#pragma once

#include "Version.h"

namespace Inscription
{
    class Scribe
    {
    public:
        virtual ~Scribe() = 0;
    protected:
        Scribe();
        Scribe(Scribe&& arg);
        Scribe& operator=(Scribe&& arg);
    private:
        Scribe(const Scribe& arg) = delete;
        Scribe& operator=(const Scribe& arg) = delete;
    };
}