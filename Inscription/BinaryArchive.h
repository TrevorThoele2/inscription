#pragma once

#include <optional>

#include "Archive.h"

#include "ScrivenDispatch.h"
#include "TableData.h"
#include "Direction.h"
#include "TypeManager.h"

namespace Inscription
{
    class OutputBinaryArchive;
    class InputBinaryArchive;

    class BinaryArchive : public Archive
    {
    private:
        using Types = TypeManager<BinaryArchive>;
    public:
        using StreamPosition = unsigned long long;
        using TypeRegistrationContext = Types::TypeRegistrationContext;
    public:
        Types types;
    public:
        BinaryArchive(const BinaryArchive& arg) = delete;
        BinaryArchive& operator=(const BinaryArchive& arg) = delete;

        virtual ~BinaryArchive() = 0;
    public:
        template<class T>
        BinaryArchive& operator()(T& object);
        template<class T>
        BinaryArchive& operator()(const std::string& name, T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputBinaryArchive* AsOutput();
        InputBinaryArchive* AsInput();
        [[nodiscard]] const OutputBinaryArchive* AsOutput() const;
        [[nodiscard]] const InputBinaryArchive* AsInput() const;
    public:
        virtual void SeekStream(StreamPosition position) = 0;
        virtual StreamPosition TellStream() = 0;
    protected:
        BinaryArchive(Direction direction);
        BinaryArchive(Direction direction, TypeRegistrationContext typeRegistrationContext);
        BinaryArchive(BinaryArchive&& arg) noexcept;

        BinaryArchive& operator=(BinaryArchive&& arg) noexcept;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    BinaryArchive& BinaryArchive::operator()(T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }

    template<class T>
    BinaryArchive& BinaryArchive::operator()(const std::string&, T& object)
    {
        ScrivenDispatch::Execute(object, *this);
        return *this;
    }

    template<class Object>
    using BinaryTableData = TableData<Object, BinaryArchive>;
}