#pragma once

#include "JsonArchive.h"
#include "OutputTextFile.h"

#include <variant>
#include <Chroma/StringUtility.h>

namespace Inscription::Archive
{
    class OutputJson final : public Json
    {
    public:
        OutputJson(const File::Path& path);
        OutputJson(const File::Path& path, const TypeRegistrationContext& typeRegistrationContext);
        OutputJson(std::string& json);
        OutputJson(std::string& json, const TypeRegistrationContext& typeRegistrationContext);
        OutputJson(OutputJson&& arg) noexcept;
        ~OutputJson();

        OutputJson& operator=(OutputJson&& arg) noexcept;

        OutputJson& WriteValue(const std::string& name, const std::string& value);
    public:
        void StartList(const std::string& name);
        void EndList();
        void StartObject(const std::string& name);
        void EndObject();
    private:
        std::variant<File::OutputText, std::string*> output;

        std::vector<uint32_t> levelCount;

        void Start();

        [[nodiscard]] static File::OutputText FileOutput(const File::Path& path);
        [[nodiscard]] static std::string* JsonOutput(std::string& json);

        void StartWrite(const std::string& name);
        void Write(const std::string& data);
        void EndWrite();

        [[nodiscard]] std::string Indent() const;
    };
}