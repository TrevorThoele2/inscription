#include "InputJsonFormat.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Format
{
    InputJson::InputJson(InputJson&& arg) noexcept :
        Json(std::move(arg)),
        rootElement(std::move(arg.rootElement))
    {}

    InputJson& InputJson::operator=(InputJson&& arg) noexcept
    {
        Json::operator=(std::move(arg));
        rootElement = std::move(arg.rootElement);
        return *this;
    }

    InputJson& InputJson::TakeValue(const std::string& name, std::string& value)
    {
        ReadValue(name, value);

        if (name.empty())
            rootElement = {};
        else
            collectionStack.back()->elements.erase(name);

        return *this;
    }

    InputJson& InputJson::ReadValue(const std::string& name, std::string& value)
    {
        if (!HasValue(name))
            throw JsonParseError(R"(The element ")" + name + R"(" could not be found.)");

        const auto& variant = name.empty()
            ? *rootElement
            : collectionStack.back()->elements.find(name)->second;
        if (!std::holds_alternative<std::string>(variant))
            throw JsonParseError(R"(The element ")" + name + R"(" was not a string.)");
        value = std::get<std::string>(variant);

        return *this;
    }

    bool InputJson::HasValue(const std::string& name)
    {
        if (name.empty())
            return collectionStack.empty() && rootElement && std::holds_alternative<std::string>(*rootElement);
        else
        {
            if (collectionStack.empty())
                return false;

            const auto elements = collectionStack.back()->elements;
            const auto found = elements.find(name);
            return found != elements.end();
        }
    }

    bool InputJson::IsNull(const std::string& name)
    {
        if (name.empty())
            return collectionStack.empty() && rootElement && std::holds_alternative<std::string>(*rootElement) && std::get<std::string>(*rootElement) == "null";
        else
        {
            if (collectionStack.empty())
                return false;

            const auto elements = collectionStack.back()->elements;
            const auto found = elements.find(name);
            return found != elements.end() && std::holds_alternative<std::string>(found->second) && std::get<std::string>(found->second) == "null";
        }
    }

    ContainerSize InputJson::StartList(const std::string& name)
    {
        NextCollection(name, CollectionType::List);
        return collectionStack.back()->elements.size();
    }

    void InputJson::EndList()
    {
        collectionStack.pop_back();
    }

    void InputJson::StartObject(const std::string& name)
    {
        NextCollection(name, CollectionType::Object);
    }

    void InputJson::EndObject()
    {
        collectionStack.pop_back();
    }

    void InputJson::NextCollection(const std::string& name, CollectionType type)
    {
        Collection* considerCollection;
        if (collectionStack.empty())
        {
            if (!rootElement || !std::holds_alternative<std::shared_ptr<Collection>>(*rootElement))
                throw JsonParseError();

            considerCollection = std::get<std::shared_ptr<Collection>>(*rootElement).get();
        }
        else
        {
            const auto elements = collectionStack.back()->elements;
            const auto found = elements.find(name);
            if (found == elements.end() || !std::holds_alternative<std::shared_ptr<Collection>>(found->second))
                throw JsonParseError();

            considerCollection = std::get<std::shared_ptr<Collection>>(found->second).get();
        }
        
        if (considerCollection->type != type)
            throw JsonParseError();

        collectionStack.push_back(considerCollection);
    }

    InputJson::Parser::Parser(const std::string& json)
    {
        if (json.empty())
            return;

        std::string useJson = json;
        const auto firstCharacter = json[0];
        switch (firstCharacter)
        {
        case '"':
            useJson.erase(0, 1);
            ParseString(useJson);
            break;
        case '[':
            useJson.erase(0, 1);
            ParseList("", useJson);
            break;
        case '{':
            useJson.erase(0, 1);
            ParseObject("", useJson);
            break;
        case ',':
        case ':':
        case '}':
        case ']':
            throw JsonParseError();
        default:
            ParseNumber(useJson);
            break;
        }
    }

    auto InputJson::Parser::RootElement() const -> std::optional<Element>
    {
        return rootElement;
    }

    void InputJson::Parser::ParseString(std::string& json)
    {
        std::string building;
        bool isEscaped = false;
        while (!json.empty())
        {
            const auto character = json[0];
            json.erase(0, 1);

            const auto wasEscaped = isEscaped;
            isEscaped = false;

            switch (character)
            {
            case '\\':
                if (wasEscaped)
                    building.push_back(character);
                else
                    isEscaped = true;

                break;
            case '"':
            {
                if (wasEscaped)
                {
                    building.push_back(character);
                    break;
                }
                else
                {
                    if (!json.empty())
                        throw JsonParseError();

                    rootElement = R"(")" + building + R"(")";
                    return;
                }
            }
            default:
                building.push_back(character);
            }
        }

        rootElement = building;
    }

    void InputJson::Parser::ParseNumber(std::string& json)
    {
        std::string building;
        while (!json.empty())
        {
            const auto character = json[0];
            json.erase(0, 1);

            switch (character)
            {
            case '"':
            case ',':
            case ':':
            case '{':
            case '}':
            case '[':
            case ']':
                throw JsonParseError();
            default:
                building.push_back(character);
            }
        }

        rootElement = building;
    }

    void InputJson::Parser::ParseList(const std::string& name, std::string& json)
    {
        const auto object = NewCollection(name, CollectionType::List);
        size_t i = 0;
        std::string building;
        bool isEscaped = false;
        while (!json.empty())
        {
            const auto character = json[0];
            json.erase(0, 1);

            const auto wasEscaped = isEscaped;
            isEscaped = false;

            switch (character)
            {
            case '\\':
                if (wasEscaped)
                    building.push_back(character);
                else
                    isEscaped = true;

                break;
            case '"':
            {
                if (wasEscaped)
                    building.push_back(character);
                else
                {
                    const auto quoted = ParseQuoted(json);
                    object->elements.emplace(Chroma::ToString(i), R"(")" + quoted + R"(")");
                    ++i;
                }
                break;
            }
            case ',':
                if (!building.empty())
                {
                    object->elements.emplace(Chroma::ToString(i), building);
                    building = {};
                    ++i;
                }
                break;
            case ':':
                throw JsonParseError();
            case '{':
                if (!building.empty())
                    throw JsonParseError();
                ParseObject(Chroma::ToString(i), json);
                ++i;
                break;
            case '}':
                throw JsonParseError();
            case '[':
                if (!building.empty())
                    throw JsonParseError();
                ParseList(Chroma::ToString(i), json);
                ++i;
                break;
            case ']':
                if (!building.empty())
                    object->elements.emplace(Chroma::ToString(i), building);
                collectionStack.pop_back();
                return;
            default:
                building.push_back(character);
            }
        }

        throw JsonParseError();
    }

    void InputJson::Parser::ParseObject(const std::string& name, std::string& json)
    {
        const auto object = NewCollection(name, CollectionType::Object);
        std::optional<std::string> buildingName;
        std::string building;
        bool isEscaped = false;
        while(!json.empty())
        {
            const auto character = json[0];
            json.erase(0, 1);

            const auto wasEscaped = isEscaped;
            isEscaped = false;

            switch (character)
            {
            case '\\':
                if (wasEscaped)
                    building.push_back(character);
                else
                    isEscaped = true;

                break;
            case '"':
            {
                if (wasEscaped)
                    building.push_back(character);
                else
                {
                    const auto quoted = ParseQuoted(json);
                    if (!buildingName)
                    {
                        if (json.empty() || json[0] != ':' || quoted.empty())
                            throw JsonParseError();
                        buildingName = quoted;
                    }
                    else
                    {
                        object->elements.emplace(*buildingName, R"(")" + quoted + R"(")");
                        buildingName = {};
                    }
                }
                break;
            }
            case ',':
                if (!building.empty() && buildingName)
                {
                    object->elements.emplace(*buildingName, building);
                    buildingName = {};
                    building = {};
                }
                break;
            case ':':
                break;
            case '{':
                if (!buildingName)
                    throw JsonParseError();
                ParseObject(*buildingName, json);
                buildingName = {};
                break;
            case '}':
                if (!building.empty())
                {
                    if (!buildingName)
                        throw JsonParseError();

                    object->elements.emplace(*buildingName, building);
                }
                collectionStack.pop_back();
                return;
            case '[':
                if (!buildingName)
                    throw JsonParseError();
                ParseList(*buildingName, json);
                buildingName = {};
                break;
            case ']':
                throw JsonParseError();
            default:
                building.push_back(character);
            }
        }

        throw JsonParseError();
    }

    std::string InputJson::Parser::ParseQuoted(std::string& json)
    {
        std::string building;
        bool isEscaped = false;
        while(!json.empty())
        {
            const auto character = json[0];
            json.erase(0, 1);

            const auto wasEscaped = isEscaped;
            isEscaped = false;

            switch (character)
            {
            case '\\':
                if (wasEscaped)
                    building.push_back(character);
                else
                    isEscaped = true;
                break;
            case '\"':
                if (wasEscaped)
                    building.push_back(character);
                else
                    return building;
                break;
            default:
                building.push_back(character);
            }
        }

        return building;
    }

    auto InputJson::Parser::NewCollection(const std::string& name, CollectionType type) -> Collection*
    {
        auto collection = std::make_unique<Collection>(type, std::unordered_map<std::string, Element>{});
        if (!rootElement)
        {
            rootElement = std::move(collection);
            collectionStack.push_back(std::get<std::shared_ptr<Collection>>(*rootElement).get());
        }
        else
        {
            const auto emplaced = collectionStack.back()->elements.emplace(name, std::move(collection)).first;
            collectionStack.push_back(std::get<std::shared_ptr<Collection>>(emplaced->second).get());
        }

        return collectionStack.back();
    }

    auto InputJson::ParseString(const std::string& json) -> std::optional<Element>
    {
        const Parser parser(json);
        return parser.RootElement();
    }
    
    std::string InputJson::ExtractArchive(Archive::InputText& archive)
    {
        std::string value;
        archive.ReadSize(value, std::numeric_limits<size_t>::max());
        return value;
    }
}