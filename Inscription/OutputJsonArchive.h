#pragma once

#include "JsonArchive.h"
#include "OutputTextFile.h"

#include <Chroma/StringUtility.h>

namespace Inscription::Archive
{
    class OutputJson final : public Json
    {
    public:
        OutputJson(const File::Path& path);
        OutputJson(const File::Path& path, const TypeRegistrationContext& typeRegistrationContext);
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
        File::OutputText file;

        std::vector<uint32_t> levelCount;

        void StartWrite(const std::string& name);
        void EndWrite();

        [[nodiscard]] std::string Indent() const;
    };
}