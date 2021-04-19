#pragma once

#include <optional>

#include "Archive.h"

#include "ScrivenDispatch.h"
#include "TableData.h"
#include "Direction.h"
#include "TypeManager.h"
#include "FilePosition.h"

namespace Inscription::Archive
{
    class OutputBinary;
    class InputBinary;

    class Binary : public Archive
    {
    private:
        using Types = TypeManager<Binary>;
    public:
        using TypeRegistrationContext = Types::TypeRegistrationContext;
    public:
        Types types;
    public:
        Binary(const Binary& arg) = delete;
        Binary& operator=(const Binary& arg) = delete;

        virtual ~Binary() = 0;
    public:
        template<class T>
        Binary& operator()(T& object);
        template<class T>
        Binary& operator()(const std::string& name, T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputBinary* AsOutput();
        InputBinary* AsInput();
        [[nodiscard]] const OutputBinary* AsOutput() const;
        [[nodiscard]] const InputBinary* AsInput() const;
    public:
        virtual void Seek(File::Position offset) = 0;
        virtual void SeekFromBeginning(File::Position offset = 0) = 0;
        virtual void SeekFromEnd(File::Position offset = 0) = 0;
        virtual File::Position Position() = 0;
    protected:
        Binary(Direction direction);
        Binary(Direction direction, TypeRegistrationContext typeRegistrationContext);
        Binary(Binary&& arg) noexcept;

        Binary& operator=(Binary&& arg) noexcept;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    Binary& Binary::operator()(T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }

    template<class T>
    Binary& Binary::operator()(const std::string&, T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }
}

namespace Inscription
{
    template<class Object>
    using BinaryTableData = TableData<Object, Archive::Binary>;
}