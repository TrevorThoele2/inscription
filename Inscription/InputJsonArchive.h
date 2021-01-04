#pragma once

#include <variant>
#include "JsonArchive.h"
#include "InputTextFile.h"

#include "JsonParseError.h"

#include "ContainerSize.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    class InputJsonArchive final : public JsonArchive
    {
    public:
        InputJsonArchive(const FilePath& path);
        InputJsonArchive(const FilePath& path, const TypeRegistrationContext& typeRegistrationContext);
        InputJsonArchive(InputJsonArchive&& arg) noexcept;

        InputJsonArchive& operator=(InputJsonArchive&& arg) noexcept;

        InputJsonArchive& TakeValue(const std::string& name, std::string& value);
        InputJsonArchive& ReadValue(const std::string& name, std::string& value);
        [[nodiscard]] bool HasValue(const std::string& name);

        ContainerSize StartList(const std::string& name);
        void EndList();
        void StartObject(const std::string& name);
        void EndObject();
    private:
        InputTextFile file;

        class Item
        {
        public:
            virtual ~Item() = 0;
        protected:
            Item() = default;
        };

        using ItemPtr = std::unique_ptr<Item>;

        class Value final : public Item
        {
        public:
            std::string string;
            Value() = default;
            explicit Value(const std::string& string);
        };

        class Collection : public Item
        {
        public:
            struct ReadResult
            {
                Collection* newCollection;
                size_t endPosition;
            };
        public:
            Collection* parent;

            virtual ~Collection() = 0;

            template<class T>
            T* StartNew(const std::string& name);

            virtual void Emplace(const std::string& name, ItemPtr&& item) = 0;
            virtual void Destroy(Item* item) = 0;

            [[nodiscard]] virtual ReadResult Read(const std::string& read) = 0;
            [[nodiscard]] virtual size_t Size() const = 0;
        protected:
            explicit Collection(Collection* parent);
        };

        class List final : public Collection
        {
        public:
            using Items = std::list<ItemPtr>;
            Items items;

            explicit List(Collection* parent);

            void Emplace(const std::string& name, ItemPtr&& item) override;
            void Destroy(Item* item) override;

            [[nodiscard]] ReadResult Read(const std::string& read) override;
            [[nodiscard]] size_t Size() const override;
        };

        class Object final : public Collection
        {
        public:
            using Items = std::unordered_map<std::string, ItemPtr>;
            Items items;

            explicit Object(Collection* parent);

            void Emplace(const std::string& name, ItemPtr&& item) override;
            void Destroy(Item* item) override;

            [[nodiscard]] ReadResult Read(const std::string& read) override;
            [[nodiscard]] size_t Size() const override;
        };

        Object baseObject;

        Collection* writeCollection = nullptr;
        Collection* readCollection = nullptr;

        void ScanFile();

        [[nodiscard]] std::optional<std::string> TakeValueFrom(const std::string& name, Collection& from);
        [[nodiscard]] std::optional<std::string> ReadValueFrom(const std::string& name, Collection& from);
        [[nodiscard]] Item* OptionalItem(const std::string& name, Collection& from);

        template<class T>
        void StartCollection(const std::string& name, Collection*& collection);
        void EndCollection(Collection*& collection);

        template<class T>
        [[nodiscard]] static T& RequiredTransformation(const std::string& name, Item& item);
        template<class T>
        [[nodiscard]] static T* OptionalTransformation(Item& item);

        [[nodiscard]] static std::string ParseName(const std::string& string);
        [[nodiscard]] static std::string ParseValue(const std::string& string);
    };

    template<class T>
    auto InputJsonArchive::Collection::StartNew(const std::string& name) -> T*
    {
        auto created = std::make_unique<T>(this);
        Emplace(name, std::move(created));
        return created.get();
    }

    template<class T>
    void InputJsonArchive::StartCollection(const std::string& name, Collection*& collection)
    {
        auto existentCollection = OptionalTransformation<T>(*OptionalItem(name, *collection));
        if (!existentCollection)
            existentCollection = collection->StartNew<T>(name);
        collection = existentCollection;
    }

    template<class T>
    T& InputJsonArchive::RequiredTransformation(const std::string& name, Item& item)
    {
        const auto check = OptionalTransformation<T>(item);
        if (!check)
            throw JsonParseError("The element \"" + name + "\" could not be found.");

        return *check;
    }

    template<class T>
    T* InputJsonArchive::OptionalTransformation(Item& item)
    {
        return dynamic_cast<T*>(&item);
    }
}