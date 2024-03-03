#include "OutputJsonFormat.h"

#include "EscapeJson.h"

namespace Inscription::Format
{
    OutputJson::OutputJson(OutputJson&& arg) noexcept : Json(std::move(arg)), archive(arg.archive)
    {}
    
    OutputJson& OutputJson::operator=(OutputJson&& arg) noexcept
    {
        Json::operator=(std::move(arg));
        archive = arg.archive;
        return *this;
    }

    OutputJson& OutputJson::WriteValue(const std::string& name, const std::string& value)
    {
        InsertCommaIfNeeded();
        Write(EscapeJson(name), value);
        IncrementCollection();
        return *this;
    }

    void OutputJson::StartList(const std::string& name)
    {
        InsertCommaIfNeeded();
        Write(EscapeJson(name), "[");
        IncrementCollection();
        StartCollection(CollectionType::List);
    }

    void OutputJson::EndList()
    {
        Write("", "]");
        EndCollection();
    }

    void OutputJson::StartObject(const std::string& name)
    {
        InsertCommaIfNeeded();
        Write(EscapeJson(name), "{");
        IncrementCollection();
        StartCollection(CollectionType::Object);
    }

    void OutputJson::EndObject()
    {
        Write("", "}");
        EndCollection();
    }

    void OutputJson::StartCollection(CollectionType type)
    {
        collectionStack.push_back({ type, 0 });
    }

    void OutputJson::EndCollection()
    {
        collectionStack.pop_back();
    }

    void OutputJson::IncrementCollection()
    {
        if (!collectionStack.empty())
            ++collectionStack.back().count;
    }

    void OutputJson::Write(const std::string& name, const std::string& string)
    {
        const auto useName = !name.empty() && IsCollection(CollectionType::Object) ? R"(")" + name + R"(":)" : "";
        const auto toWrite = useName + string;
        archive->Write(toWrite);
    }
    
    void OutputJson::InsertCommaIfNeeded()
    {
        if (!collectionStack.empty() && collectionStack.back().count > 0)
            Write("", ",");
    }
    
    bool OutputJson::IsCollection(CollectionType type) const
    {
        return !collectionStack.empty() && collectionStack.back().type == type;
    }
}