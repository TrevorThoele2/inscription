#pragma once

#include "Archive.h"

#include "ScrivenDispatch.h"
#include "Direction.h"
#include "Const.h"

namespace Inscription::Archive
{
    class OutputText;
    class InputText;

    class Text : public Archive
    {
    public:
        virtual ~Text() = 0;
    public:
        template<class T>
        Text& operator()(T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputText* AsOutput();
        InputText* AsInput();
        [[nodiscard]] const OutputText* AsOutput() const;
        [[nodiscard]] const InputText* AsInput() const;
    protected:
        explicit Text(Direction direction);
        Text(Text&& arg) noexcept;

        Text& operator=(Text&& arg) noexcept;
    private:
        const Direction direction;
    };

    template<class T>
    Text& Text::operator()(T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }
}