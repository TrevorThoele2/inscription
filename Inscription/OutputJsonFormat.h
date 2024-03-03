#pragma once

#include "JsonFormat.h"
#include "OutputTextArchive.h"

#include <variant>
#include <Chroma/StringUtility.h>

namespace Inscription::Format
{
    class OutputJson final : public Json
    {
    public:
        template<class T>
        OutputJson(T& value, Archive::OutputText& archive, const TypeRegistrationContext& typeRegistrationContext);
        OutputJson(OutputJson&& arg) noexcept;

        OutputJson& operator=(OutputJson&& arg) noexcept;

        OutputJson& WriteValue(const std::string& name, const std::string& value);
    public:
        void StartList(const std::string& name);
        void EndList();
        void StartObject(const std::string& name);
        void EndObject();
    private:
        Archive::OutputText* archive = nullptr;

        enum class CollectionType
        {
            List,
            Object
        };

        struct Collection
        {
            CollectionType type;
            size_t count;
        };

        std::vector<Collection> collectionStack;

        void StartCollection(CollectionType type);
        void EndCollection();
        void IncrementCollection();
        void Write(const std::string& name, const std::string& string);
        void InsertCommaIfNeeded();
        [[nodiscard]] bool IsCollection(CollectionType type) const;
    };

    template<class T>
    OutputJson::OutputJson(
        T& value, Archive::OutputText& archive, const TypeRegistrationContext& typeRegistrationContext)
	    :
		Json(Direction::Output, typeRegistrationContext),
		archive(&archive)
    {
        (*this)("", value);
    }
}