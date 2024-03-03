#include "OutputJsonArchive.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Archive
{
    OutputJson::OutputJson(
        const File::Path& path)
		:
        Json(Direction::Output),
        output(FileOutput(path))
    {
        Start();
    }

    OutputJson::OutputJson(
        const File::Path& path, const TypeRegistrationContext& typeRegistrationContext)
        :
        Json(Direction::Output, typeRegistrationContext),
        output(FileOutput(path))
    {
        Start();
    }

    OutputJson::OutputJson(
        std::string& json)
	    :
        Json(Direction::Output),
		output(JsonOutput(json))
    {
        Start();
    }

    OutputJson::OutputJson(
        std::string& json, const TypeRegistrationContext& typeRegistrationContext)
	    :
        Json(Direction::Output, typeRegistrationContext),
        output(JsonOutput(json))
    {
        Start();
    }

    OutputJson::OutputJson(OutputJson&& arg) noexcept :
        Json(std::move(arg)), output(std::move(arg.output))
    {}

    OutputJson::~OutputJson()
    {
        if (levelCount.back() > 0)
            Write("\n}");
        else
            Write("}");
    }

    OutputJson& OutputJson::operator=(OutputJson&& arg) noexcept
    {
        Json::operator=(std::move(arg));
        output = std::move(arg.output);
        return *this;
    }

    OutputJson& OutputJson::WriteValue(const std::string& name, const std::string& value)
    {
        StartWrite(name);
        Write(value);
        EndWrite();

        return *this;
    }

    void OutputJson::StartList(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        Write("[");
    }

    void OutputJson::EndList()
    {
        levelCount.pop_back();
        Write("\n" + Indent() + "]");

        EndWrite();
    }

    void OutputJson::StartObject(const std::string& name)
    {
        StartWrite(name);

        levelCount.push_back(0);
        Write("{");
    }

    void OutputJson::EndObject()
    {
        levelCount.pop_back();
        Write("\n" + Indent() + "}");

        EndWrite();
    }

    void OutputJson::Start()
    {
        Write("{");
        levelCount.push_back(0);
    }

	File::OutputText OutputJson::FileOutput(const File::Path& path)
    {
        return File::OutputText(path);
    }

	std::string* OutputJson::JsonOutput(std::string& json)
    {
        return &json;
    }

    void OutputJson::StartWrite(const std::string& name)
    {
        if (levelCount.back() > 0)
            Write(",");
        Write("\n" + Indent());
        if(!name.empty())
            Write("\"" + name + "\": ");
    }

    void OutputJson::Write(const std::string& data)
    {
        if (std::holds_alternative<File::OutputText>(output))
            std::get<File::OutputText>(output).WriteData(data);
        else
            *std::get<std::string*>(output) += data;
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