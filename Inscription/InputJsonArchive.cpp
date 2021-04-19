#include "InputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Archive
{
    InputJson::InputJson(const File::Path& path) :
        Json(Direction::Input),
        file(path),
        baseObject(nullptr),
        writeCollection(&baseObject),
        readCollection(&baseObject)
    {
        char character;
        file.ReadData(character);
    }

    InputJson::InputJson(
        const File::Path& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        Json(Direction::Input, typeRegistrationContext),
        file(path),
        baseObject(nullptr),
        writeCollection(&baseObject),
        readCollection(&baseObject)
    {
        char character;
        file.ReadData(character);
    }

    InputJson::InputJson(InputJson&& arg) noexcept :
        Json(std::move(arg)),
        file(std::move(arg.file)),
        baseObject(std::move(arg.baseObject)),
        writeCollection(&baseObject),
        readCollection(&baseObject)
    {}

    InputJson& InputJson::operator=(InputJson&& arg) noexcept
    {
        Json::operator=(std::move(arg));
        file = std::move(arg.file);
        baseObject = std::move(arg.baseObject);
        writeCollection = &baseObject;
        readCollection = &baseObject;
        return *this;
    }

    InputJson& InputJson::TakeValue(const std::string& name, std::string& value)
    {
        auto readValue = TakeValueFrom(name, *readCollection);
        if (!readValue)
            throw JsonParseError("The element \"" + name + "\" could not be found.");
        value = *readValue;

        return *this;
    }

    InputJson& InputJson::ReadValue(const std::string& name, std::string& value)
    {
        auto readValue = ReadValueFrom(name, *readCollection);
        if (!readValue)
            throw JsonParseError("The element \"" + name + "\" could not be found.");
        value = *readValue;

        return *this;
    }

    bool InputJson::HasValue(const std::string& name)
    {
        return static_cast<bool>(OptionalItem(name, *readCollection));
    }

    ContainerSize InputJson::StartList(const std::string& name)
    {
        StartCollection<List>(name, readCollection);
        return readCollection->Size();
    }

    void InputJson::EndList()
    {
        EndCollection(readCollection);
    }

    void InputJson::StartObject(const std::string& name)
    {
        StartCollection<Object>(name, readCollection);
    }

    void InputJson::EndObject()
    {
        EndCollection(readCollection);
    }

    InputJson::Item::~Item() = default;

    InputJson::Value::Value(const std::string& string) : string(string)
    {}

    InputJson::Collection::~Collection() = default;

    InputJson::Collection::Collection(Collection* parent) : parent(parent)
    {}

    InputJson::List::List(Collection* parent) : Collection(parent)
    {}

    void InputJson::List::Emplace(const std::string& name, ItemPtr&& item)
    {
        if (!name.empty())
            throw JsonParseError("Lists do not use names (attempted name: " + name + ").");

        items.push_back(std::move(item));
    }

    void InputJson::List::Destroy(Item* item)
    {
        for (auto currentItem = items.begin(); currentItem != items.end(); ++currentItem)
        {
            if (currentItem->get() == item)
            {
                items.erase(currentItem);
                return;
            }
        }
    }

    auto InputJson::List::Read(const std::string& read) -> ReadResult
    {
        std::string valueElement;
        auto readingString = false;
        auto escapeNext = false;
        for(size_t i = 0; i < read.size(); ++i)
        {
            const auto character = read[i];

            if (escapeNext)
            {
                valueElement += character;
                escapeNext = false;
            }
            else if (character == '\\')
                escapeNext = true;
            else if (character == '\"')
            {
                valueElement += character;
                readingString = !readingString;
            }
            else if (readingString)
                valueElement += character;
            else
            {
                switch (character)
                {
                case ' ':
                    break;
                case '\n':
                    break;
                case '\r':
                    break;
                case '{':
                {
                    if (!valueElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<Object>(this);
                    const auto collection = created.get();
                    items.push_back(std::move(created));
                    return { collection, i + 1 };
                }
                case '[':
                {
                    if (!valueElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<List>(this);
                    const auto collection = created.get();
                    items.push_back(std::move(created));
                    return { collection, i + 1 };
                }
                case ']':
                {
                    if (!valueElement.empty())
                    {
                        auto created = std::make_unique<Value>(valueElement);
                        items.push_back(std::move(created));
                    }

                    return { nullptr, i + 1 };
                }
                case ',':
                {
                    if (valueElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<Value>(valueElement);
                    items.push_back(std::move(created));
                    valueElement = "";
                    break;
                }
                default:
                    valueElement += character;
                }
            }
        }

        return { nullptr, read.size() };
    }

    size_t InputJson::List::Size() const
    {
        return items.size();
    }

    InputJson::Object::Object(Collection* parent) : Collection(parent)
    {}

    void InputJson::Object::Emplace(const std::string& name, ItemPtr&& item)
    {
        if (name.empty())
            throw JsonParseError();

        items.emplace(name, std::move(item));
    }

    void InputJson::Object::Destroy(Item* item)
    {
        for (auto currentItem = items.begin(); currentItem != items.end(); ++currentItem)
        {
            if (currentItem->second.get() == item)
            {
                items.erase(currentItem);
                return;
            }
        }
    }

    auto InputJson::Object::Read(const std::string& read) -> ReadResult
    {
        std::string readElement;
        std::string nameElement;
        auto readingString = false;
        auto escapeNext = false;
        for (size_t i = 0; i < read.size(); ++i)
        {
            const auto character = read[i];

            if (escapeNext)
            {
                readElement += character;
                escapeNext = false;
            }
            else if (character == '\\')
                escapeNext = true;
            else if (character == '\"')
            {
                readElement += character;
                readingString = !readingString;
            }
            else if (readingString)
                readElement += character;
            else
            {
                switch (character)
                {
                case ' ':
                    break;
                case '\n':
                    break;
                case '\r':
                    break;
                case '{':
                {
                    if (nameElement.empty())
                        throw JsonParseError("Value on object requires name.");
                    if (!readElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<Object>(this);
                    const auto collection = created.get();
                    items.emplace(nameElement, std::move(created));
                    return { collection, i + 1 };
                }
                case '}':
                {
                    if (!readElement.empty() && nameElement.empty())
                        throw JsonParseError("Value on object requires name.");
                    else if (readElement.empty() && !nameElement.empty())
                        throw JsonParseError("Name on object does not have value.");
                    else if (!readElement.empty() && !nameElement.empty())
                    {
                        auto created = std::make_unique<Value>(readElement);
                        items.emplace(nameElement, std::move(created));
                    }

                    return { nullptr, i + 1 };
                }
                case '[':
                {
                    if (nameElement.empty())
                        throw JsonParseError("Value on object requires name.");
                    if (!readElement.empty() || nameElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<List>(this);
                    const auto collection = created.get();
                    items.emplace(nameElement, std::move(created));
                    return { collection, i + 1 };
                }
                case ':':
                {
                    if (readElement.size() < 3)
                        throw JsonParseError();

                    if (readElement[0] != '\"' || readElement[readElement.size() - 1] != '\"')
                        throw JsonParseError();

                    nameElement = readElement.substr(1, readElement.size() - 2);
                    readElement = "";
                    break;
                }
                case ',':
                {
                    if (readElement.empty() || nameElement.empty())
                        throw JsonParseError();

                    auto created = std::make_unique<Value>(readElement);
                    items.emplace(nameElement, std::move(created));
                    readElement = "";
                    break;
                }
                default:
                    readElement += character;
                }
            }
        }

        return { nullptr, read.size() };
    }

    size_t InputJson::Object::Size() const
    {
        return items.size();
    }

    void InputJson::ScanFile()
    {
        if (file.IsAtEnd())
            return;

        auto read = file.ReadSize(std::numeric_limits<size_t>::max());

        while (!read.empty())
        {
            const auto result = writeCollection->Read(read);
            read.erase(0, result.endPosition);
            if (!read.empty() && read[0] == ',')
                read.erase(0, 1);

            if (!read.empty() && read[0] == ',')
                throw JsonParseError();

            if (result.newCollection)
                writeCollection = result.newCollection;
            else
                writeCollection = writeCollection->parent;
        }
    }

    std::optional<std::string> InputJson::TakeValueFrom(const std::string& name, Collection& from)
    {
        const auto findItem = [name, &from]() -> std::optional<std::string>
        {
            if (name.empty())
            {
                auto& list = RequiredTransformation<List>(name, from);
                const auto item = list.items.front().get();
                if (!item)
                    return {};

                const auto castedItem = OptionalTransformation<Value>(*item);
                if (castedItem)
                {
                    auto returnValue = castedItem->string;
                    list.items.pop_front();
                    return returnValue;
                }
                else
                    return {};
            }
            else
            {
                auto& object = RequiredTransformation<Object>(name, from);
                const auto found = object.items.find(name);
                if (found == object.items.end())
                    return {};

                const auto item = found->second.get();
                if (!item)
                    return {};

                const auto castedItem = OptionalTransformation<Value>(*item);
                if (castedItem)
                {
                    auto returnValue = castedItem->string;
                    object.items.erase(found);
                    return returnValue;
                }
                else
                    return {};
            }
        };

        auto string = findItem();

        if (!string)
        {
            ScanFile();
            string = findItem();
            if (!string)
                return {};
        }

        return *string;
    }

    std::optional<std::string> InputJson::ReadValueFrom(const std::string& name, Collection& from)
    {
        const auto findItem = [name, &from]() -> std::optional<std::string>
        {
            if (name.empty())
            {
                auto& list = RequiredTransformation<List>(name, from);
                const auto item = list.items.front().get();
                if (!item)
                    return {};

                const auto castedItem = OptionalTransformation<Value>(*item);
                if (castedItem)
                    return castedItem->string;
                else
                    return {};
            }
            else
            {
                auto& object = RequiredTransformation<Object>(name, from);
                const auto found = object.items.find(name);
                if (found == object.items.end())
                    return {};

                const auto item = found->second.get();
                if (!item)
                    return {};

                const auto castedItem = OptionalTransformation<Value>(*item);
                if (castedItem)
                    return castedItem->string;
                else
                    return {};
            }
        };

        auto string = findItem();

        if (!string)
        {
            ScanFile();
            string = findItem();
            if (!string)
                return {};
        }

        return *string;
    }

    auto InputJson::OptionalItem(const std::string& name, Collection& from) -> Item*
    {
        const auto findItem = [name, &from]()
        {
            if (name.empty())
            {
                auto& list = RequiredTransformation<List>(name, from);
                return list.items.front().get();
            }
            else
            {
                auto& object = RequiredTransformation<Object>(name, from);
                const auto found = object.items.find(name);
                return found != object.items.end()
                    ? found->second.get()
                    : nullptr;
            }
        };

        {
            const auto item = findItem();
            if (item)
                return item;
        }

        ScanFile();
        return findItem();
    }

    void InputJson::EndCollection(Collection*& collection)
    {
        const auto destroyCollection = collection;
        collection = collection->parent;
        collection->Destroy(destroyCollection);
    }

    std::string InputJson::ParseName(const std::string& string)
    {
        if (string.empty() || string[string.size() - 1] != '\"')
            throw JsonParseError();

        const auto start = string.rfind('\"', string.size() - 2);
        return string.substr(start + 1, string.size() - 2 - start);
    }

    std::string InputJson::ParseValue(const std::string& string)
    {
        if (string.empty())
            throw JsonParseError();

        const auto end = string.find_last_of(',');
        return string.substr(0, end);
    }
}