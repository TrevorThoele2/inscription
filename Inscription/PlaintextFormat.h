#pragma once

#include "Format.h"

#include "ScrivenDispatch.h"
#include "TableData.h"
#include "Direction.h"
#include "TypeManager.h"
#include "TextArchive.h"

namespace Inscription::Format
{
    class OutputPlaintext;
    class InputPlaintext;

    class Plaintext : public Format
    {
    private:
        using Types = TypeManager<Plaintext>;
    public:
        using TypeRegistrationContext = Types::TypeRegistrationContext;
    public:
        Types types;
    public:
        Plaintext(const Plaintext& arg) = delete;
        Plaintext& operator=(const Plaintext& arg) = delete;

        virtual ~Plaintext() = 0;
    public:
        template<class T>
        Plaintext& operator()(T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputPlaintext* AsOutput();
        InputPlaintext* AsInput();
        [[nodiscard]] const OutputPlaintext* AsOutput() const;
        [[nodiscard]] const InputPlaintext* AsInput() const;
    protected:
        Plaintext(Direction direction);
        Plaintext(Direction direction, TypeRegistrationContext typeRegistrationContext);
        Plaintext(Plaintext&& arg) noexcept;

        Plaintext& operator=(Plaintext&& arg) noexcept;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    Plaintext& Plaintext::operator()(T& object)
    {
        ScrivenDispatch::Execute(RemoveConst(object), *this);
        return *this;
    }
}

namespace Inscription
{
    template<class Object>
    using PlaintextTableData = TableData<Object, Format::Plaintext>;
}