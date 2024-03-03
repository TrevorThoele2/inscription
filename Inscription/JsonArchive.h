#pragma once

#include <optional>

#include "Archive.h"

#include "ScrivenDispatch.h"
#include "TableData.h"
#include "Direction.h"
#include "TypeManager.h"

namespace Inscription::Archive
{
    class OutputJson;
    class InputJson;

    class Json : public Archive
    {
    private:
        using Types = TypeManager<Json>;
    public:
        using TypeRegistrationContext = Types::TypeRegistrationContext;
    public:
        Types types;
    public:
        Json(const Json& arg) = delete;
        Json& operator=(const Json& arg) = delete;

        virtual ~Json() = 0;
    public:
        template<class T>
        Json& operator()(const std::string& name, T& object);
    public:
        [[nodiscard]] bool IsOutput() const;
        [[nodiscard]] bool IsInput() const;

        OutputJson* AsOutput();
        InputJson* AsInput();
        [[nodiscard]] const OutputJson* AsOutput() const;
        [[nodiscard]] const InputJson* AsInput() const;
    protected:
        Json(Direction direction);
        Json(Direction direction, TypeRegistrationContext typeRegistrationContext);
        Json(Json&& arg) noexcept;

        Json& operator=(Json&& arg) noexcept;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    Json& Json::operator()(const std::string& name, T& object)
    {
        ScrivenDispatch::NamedExecute(name, RemoveConst(object), *this);
        return *this;
    }
}

namespace Inscription
{
    template<class Object>
    using JsonTableData = TableData<Object, Archive::Json>;
}