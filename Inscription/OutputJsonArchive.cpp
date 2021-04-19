#include "OutputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Archive
{
    OutputJson::OutputJson(const File::Path& path) :
        Json(Direction::Output),
        file(path)
    {
        file.WriteData("{");
        levelCount.push_back(0);
    }

    OutputJson::OutputJson(
        const File::Path& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        Json(Direction::Output, typeRegistrationContext),
        file(path)
    {
        file.WriteData("{");
        levelCount.push_back(0);
    }

    OutputJson::OutputJson(OutputJson&& arg) noexcept :
        Json(std::move(arg)), file(std::move(arg.file))
    {}

    OutputJson::~OutputJson()
    {
        if (levelCount.back() > 0)
            file.WriteData("\n}");
        else
            file.WriteData("}");
    }

    OutputJson& OutputJson::operator=(OutputJson&& arg) noexcept
    {
        Json::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    OutputJson& OutputJson::WriteValue(const std::string& name, const std::string& value)
    {
        StartWrite(name);
        file.WriteData(value);
        EndWrite();

        return *this;
    }

    void OutputJson::StartList(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        file.WriteData("[");
    }

    void OutputJson::EndList()
    {
        levelCount.pop_back();
        file.WriteData("\n" + Indent() + "]");

        EndWrite();
    }

    void OutputJson::StartObject(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        file.WriteData("{");
    }

    void OutputJson::EndObject()
    {
        levelCount.pop_back();
        file.WriteData("\n" + Indent() + "}");

        EndWrite();
    }

    void OutputJson::StartWrite(const std::string& name)
    {
        if (levelCount.back() > 0)
            file.WriteData(",");
        file.WriteData("\n" + Indent());
        if(!name.empty())
            file.WriteData("\"" + name + "\": ");
    }

    void OutputJson::EndWrite()
    {
        ++levelCount.back();
    }

    std::string OutputJson::Indent() const
    {
        return std::string(levelCount.size() * 4, ' ');
    }
}