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
    using TypeRegistrationContext = Inscription::TypeRegistrationContext<Inscription::JsonArchive>;
    TypeRegistrationContext typeRegistrationContext;

    using OutputArchive = Inscription::OutputJsonArchive;
    using InputArchive = Inscription::InputJsonArchive;
    using OutputTextArchive = Inscription::OutputTextArchive;

    template<class T>
    [[nodiscard]] T CreateRegistered() const;
    template<>
    [[nodiscard]] OutputArchive CreateRegistered() const;
    template<>
    [[nodiscard]] InputArchive CreateRegistered() const;
    template<>
    [[nodiscard]] OutputTextArchive CreateRegistered() const;
};

template<class T>
T JsonFixture::CreateRegistered() const
{
    static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
}

template<>
auto JsonFixture::CreateRegistered() const -> OutputArchive
{
    return OutputArchive("Test.json", typeRegistrationContext);
}

template<>
auto JsonFixture::CreateRegistered() const -> InputArchive
{
    return InputArchive("Test.json", typeRegistrationContext);
}

template<>
Inscription::OutputTextArchive JsonFixture::CreateRegistered() const
{
    return Inscription::OutputTextArchive("Test.json");
}