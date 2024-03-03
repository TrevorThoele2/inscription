#pragma once

#include <string>

#include "Archive.h"

#include "Scribe.h"

#include "Direction.h"
#include "Const.h"

namespace Inscription
{
    class OutputTextArchive;
    class InputTextArchive;

    class TextArchive : public Archive
    {
    public:
        virtual ~TextArchive() = 0;
    public:
        template<class T>
        TextArchive& operator()(T& object);
    public:
        bool IsOutput() const;
        bool IsInput() const;

        OutputTextArchive* AsOutput();
        InputTextArchive* AsInput();
        const OutputTextArchive* AsOutput() const;
        const InputTextArchive* AsInput() const;
    protected:
        TextArchive(Direction direction);
        TextArchive(TextArchive&& arg);

        TextArchive& operator=(TextArchive&& arg);
    private:
        template<class T>
        using KnownScribe = Scribe<T, TextArchive>;
    private:
        const Direction direction;
    };

    template<class T>
    TextArchive& TextArchive::operator()(T& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type> scribe;
        scribe.Scriven(RemoveConst(object), *this);
        return *this;
    }
}