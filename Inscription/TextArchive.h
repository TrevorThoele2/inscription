#pragma once

#include "Archive.h"

#include "ScrivenDispatch.h"
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
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputTextArchive* AsOutput();
        InputTextArchive* AsInput();
        [[nodiscard]] const OutputTextArchive* AsOutput() const;
        [[nodiscard]] const InputTextArchive* AsInput() const;
    protected:
        explicit TextArchive(Direction direction);
        TextArchive(TextArchive&& arg) noexcept;

        TextArchive& operator=(TextArchive&& arg) noexcept;
    private:
        const Direction direction;
    };

    template<class T>
    TextArchive& TextArchive::operator()(T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }
}