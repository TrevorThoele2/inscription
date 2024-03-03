#pragma once

#include <optional>

#include "Archive.h"

#include "ScrivenDispatch.h"
#include "TableData.h"
#include "Direction.h"
#include "TypeManager.h"
#include "StreamPosition.h"

namespace Inscription
{
    class OutputJsonArchive;
    class InputJsonArchive;

    class JsonArchive : public Archive
    {
    private:
        using Types = TypeManager<JsonArchive>;
    public:
        using StreamPosition = StreamPosition;
        using TypeRegistrationContext = Types::TypeRegistrationContext;
    public:
        Types types;
    public:
        JsonArchive(const JsonArchive& arg) = delete;
        JsonArchive& operator=(const JsonArchive& arg) = delete;

        virtual ~JsonArchive() = 0;
    public:
        template<class T>
        JsonArchive& operator()(const std::string& name, T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputJsonArchive* AsOutput();
        InputJsonArchive* AsInput();
        [[nodiscard]] const OutputJsonArchive* AsOutput() const;
        [[nodiscard]] const InputJsonArchive* AsInput() const;
    protected:
        JsonArchive(Direction direction);
        JsonArchive(Direction direction, TypeRegistrationContext typeRegistrationContext);
        JsonArchive(JsonArchive&& arg) noexcept;

        JsonArchive& operator=(JsonArchive&& arg) noexcept;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    JsonArchive& JsonArchive::operator()(const std::string& name, T& object)
    {
        ScrivenDispatch::NamedExecute(name, RemoveConst(object), *this);
        return *this;
    }

    template<class Object>
    using JsonTableData = TableData<Object, JsonArchive>;
}
