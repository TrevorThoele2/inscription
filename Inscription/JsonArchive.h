#pragma once

#include <optional>

#include "Archive.h"

#include "TypeManager.h"

#include "Scribe.h"
#include "TableData.h"

#include "Version.h"
#include "Direction.h"
#include "Const.h"

namespace Inscription
{
    class OutputJsonArchive;
    class InputJsonArchive;

    class JsonArchive : public Archive
    {
    private:
        using Types = TypeManager<JsonArchive>;
    public:
        using StreamPosition = unsigned long long;
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
        template<class T>
        JsonArchive& operator()(const std::string& name, T*& object);
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
        template<class T>
        using KnownScribe = Scribe<T, JsonArchive>;
    private:
        const Direction direction;
    private:
        template<class Archive>
        friend class PolymorphicManager;
    };

    template<class T>
    JsonArchive& JsonArchive::operator()(const std::string& name, T& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type> scribe;
        scribe.Scriven(name, RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    JsonArchive& JsonArchive::operator()(const std::string& name, T*& object)
    {
        KnownScribe<typename RemoveConstTrait<T>::type> scribe;
        scribe.Scriven(name, RemoveConst(object), *this);
        return *this;
    }

    template<class T>
    using JsonScribe = Scribe<T, JsonArchive>;
    template<class Object>
    using JsonTableData = TableData<Object, JsonArchive>;
}