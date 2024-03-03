#pragma once

#include "Version.h"

#include "RegisteredTypes.h"

namespace Inscription
{
    class Scribe
    {
    public:
        virtual ~Scribe() = 0;
    public:
        template<class T>
        static void RegisterType();
    protected:
        Scribe();
        Scribe(Scribe&& arg);
        Scribe& operator=(Scribe&& arg);
    private:
        Scribe(const Scribe& arg) = delete;
        Scribe& operator=(const Scribe& arg) = delete;
    };

    template<class T>
    void Scribe::RegisterType()
    {
        RegisteredTypes::Register<T>();
    }
}