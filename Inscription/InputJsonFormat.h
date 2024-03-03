#pragma once

#include <variant>
#include "JsonFormat.h"
#include "InputTextArchive.h"

#include "JsonParseError.h"

#include "ContainerSize.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Format
{
    class InputJson final : public Json
    {
    public:
        template<class T>
        InputJson(T& value, Archive::InputText& archive, const TypeRegistrationContext& typeRegistrationContext = TypeRegistrationContext());
        InputJson(InputJson&& arg) noexcept;

        InputJson& operator=(InputJson&& arg) noexcept;
    public:
        InputJson& TakeValue(const std::string& name, std::string& value);
        InputJson& ReadValue(const std::string& name, std::string& value);
        [[nodiscard]] bool HasValue(const std::string& name);
        [[nodiscard]] bool IsNull(const std::string& name);

        ContainerSize StartList(const std::string& name);
        void EndList();
        void StartObject(const std::string& name);
        void EndObject();
    private:
        struct Collection;

        using Element = std::variant<std::string, std::shared_ptr<Collection>>;

        enum class CollectionType
        {
            List,
            Object
        };

        struct Collection
        {
            CollectionType type;
            std::unordered_map<std::string, Element> elements;
        };
        
        std::optional<Element> rootElement;
        std::vector<Collection*> collectionStack;

        void NextCollection(const std::string& name, CollectionType type);
    private:
        class Parser
        {
        public:
            explicit Parser(const std::string& json);
            [[nodiscard]] std::optional<Element> RootElement() const;
        private:
            std::optional<Element> rootElement;
            std::vector<Collection*> collectionStack;

            void ParseString(std::string& json);
            void ParseNumber(std::string& json);
            void ParseList(const std::string& name, std::string& json);
            void ParseObject(const std::string& name, std::string& json);
            [[nodiscard]] static std::string ParseQuoted(std::string& json);

            Collection* NewCollection(const std::string& name, CollectionType type);
        };

        [[nodiscard]] static std::optional<Element> ParseString(const std::string& json);
        [[nodiscard]] static std::string ExtractArchive(Archive::InputText& archive);
    };

    template<class T>
    InputJson::InputJson(T& value, Archive::InputText& archive, const TypeRegistrationContext& typeRegistrationContext) :
        Json(Direction::Input, typeRegistrationContext),
        rootElement(ParseString(ExtractArchive(archive)))
    {
        (*this)("", value);
    }
}