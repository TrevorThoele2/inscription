#pragma once

#include "JsonArchive.h"
#include "OutputTextFile.h"

#include <Chroma/StringUtility.h>

namespace Inscription
{
    class OutputJsonArchive final : public JsonArchive
    {
    public:
        OutputJsonArchive(const FilePath& path);
        OutputJsonArchive(const FilePath& path, const TypeRegistrationContext& typeRegistrationContext);
        OutputJsonArchive(OutputJsonArchive&& arg) noexcept;
        ~OutputJsonArchive();

        OutputJsonArchive& operator=(OutputJsonArchive&& arg) noexcept;

        OutputJsonArchive& WriteValue(const std::string& name, const std::string& value);
    public:
        void StartList(const std::string& name);
        void EndList();
        void StartObject(const std::string& name);
        void EndObject();
    private:
        OutputTextFile file;

        std::vector<uint32_t> levelCount;

        void StartWrite(const std::string& name);
        void EndWrite();

        [[nodiscard]] std::string Indent() const;
    };
}