#include "OutputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    OutputJsonArchive::OutputJsonArchive(const FilePath& path) :
        JsonArchive(Direction::Output),
        file(path)
    {
        file.WriteData("{");
        levelCount.push_back(0);
    }

    OutputJsonArchive::OutputJsonArchive(
        const FilePath& path, const TypeRegistrationContext& typeRegistrationContext) :
        JsonArchive(Direction::Output, typeRegistrationContext),
        file(path)
    {
        file.WriteData("{");
        levelCount.push_back(0);
    }

    OutputJsonArchive::OutputJsonArchive(OutputJsonArchive&& arg) noexcept :
        JsonArchive(std::move(arg)), file(std::move(arg.file))
    {}

    OutputJsonArchive::~OutputJsonArchive()
    {
        if (levelCount.back() > 0)
            file.WriteData("\n}");
        else
            file.WriteData("}");
    }

    OutputJsonArchive& OutputJsonArchive::operator=(OutputJsonArchive&& arg) noexcept
    {
        JsonArchive::operator=(std::move(arg));
        file = std::move(arg.file);
        return *this;
    }

    OutputJsonArchive& OutputJsonArchive::WriteValue(const std::string& name, const std::string& value)
    {
        StartWrite(name);
        file.WriteData(value);
        EndWrite();

        return *this;
    }

    void OutputJsonArchive::StartList(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        file.WriteData("[");
    }

    void OutputJsonArchive::EndList()
    {
        levelCount.pop_back();
        file.WriteData("\n" + Indent() + "]");

        EndWrite();
    }

    void OutputJsonArchive::StartObject(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        file.WriteData("{");
    }

    void OutputJsonArchive::EndObject()
    {
        levelCount.pop_back();
        file.WriteData("\n" + Indent() + "}");

        EndWrite();
    }

    void OutputJsonArchive::StartWrite(const std::string& name)
    {
        if (levelCount.back() > 0)
            file.WriteData(",");
        file.WriteData("\n" + Indent());
        if(!name.empty())
            file.WriteData("\"" + name + "\": ");
    }

    void OutputJsonArchive::EndWrite()
    {
        ++levelCount.back();
    }

    std::string OutputJsonArchive::Indent() const
    {
        return std::string(levelCount.size() * 4, ' ');
    }
}