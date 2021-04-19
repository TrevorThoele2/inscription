#pragma once

#include "GeneralFixture.h"

#include <Inscription/TypeRegistrationContext.h>

#include <Inscription/OutputJsonArchive.h>
#include <Inscription/InputJsonArchive.h>
#include <Inscription/OutputTextArchive.h>

#include <Inscription/Access.h>

class JsonFixture : public GeneralFixture
{
public:
    using TypeRegistrationContext = Inscription::TypeRegistrationContext<Inscription::Archive::Json>;
    TypeRegistrationContext typeRegistrationContext;

    using OutputArchive = Inscription::Archive::OutputJson;
    using InputArchive = Inscription::Archive::InputJson;
    using OutputTextArchive = Inscription::Archive::OutputText;

    template<class T, std::enable_if_t<std::is_same_v<T, OutputArchive>, int> = 0>
    [[nodiscard]] OutputArchive CreateRegistered() const;
    template<class T, std::enable_if_t<std::is_same_v<T, InputArchive>, int> = 0>
    [[nodiscard]] InputArchive CreateRegistered() const;
    template<class T, std::enable_if_t<std::is_same_v<T, OutputTextArchive>, int> = 0>
    [[nodiscard]] OutputTextArchive CreateRegistered() const;
};

template<class T, std::enable_if_t<std::is_same_v<T, JsonFixture::OutputArchive>, int>>
auto JsonFixture::CreateRegistered() const -> OutputArchive
{
    return OutputArchive("Test.json", typeRegistrationContext);
}

template<class T, std::enable_if_t<std::is_same_v<T, JsonFixture::InputArchive>, int>>
auto JsonFixture::CreateRegistered() const -> InputArchive
{
    return InputArchive("Test.json", typeRegistrationContext);
}

template<class T, std::enable_if_t<std::is_same_v<T, JsonFixture::OutputTextArchive>, int>>
auto JsonFixture::CreateRegistered() const -> OutputTextArchive
{
    return OutputTextArchive("Test.json");
}