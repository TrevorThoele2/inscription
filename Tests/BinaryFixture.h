#pragma once

#include "GeneralFixture.h"

#include <Inscription/TypeRegistrationContext.h>

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

#include <Inscription/Access.h>

class BinaryFixture : public GeneralFixture
{
public:
    using TypeRegistrationContext = Inscription::TypeRegistrationContext<Inscription::BinaryArchive>;
    TypeRegistrationContext typeRegistrationContext;

    using OutputArchive = Inscription::OutputBinaryArchive;
    using InputArchive = Inscription::InputBinaryArchive;

    template<class T>
    [[nodiscard]] T CreateRegistered() const;
    template<>
    [[nodiscard]] OutputArchive CreateRegistered() const;
    template<>
    [[nodiscard]] InputArchive CreateRegistered() const;
};

template<class T>
T BinaryFixture::CreateRegistered() const
{
    static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
}

template<>
auto BinaryFixture::CreateRegistered() const -> OutputArchive
{
    return OutputArchive("Test.dat", typeRegistrationContext);
}

template<>
auto BinaryFixture::CreateRegistered() const -> InputArchive
{
    return InputArchive("Test.dat", typeRegistrationContext);
}