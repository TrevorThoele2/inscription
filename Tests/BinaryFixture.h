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

    template<class T, std::enable_if_t<std::is_same_v<T, OutputArchive>, int> = 0>
    [[nodiscard]] OutputArchive CreateRegistered() const;
    template<class T, std::enable_if_t<std::is_same_v<T, InputArchive>, int> = 0>
    [[nodiscard]] InputArchive CreateRegistered() const;
};

template<class T, std::enable_if_t<std::is_same_v<T, BinaryFixture::OutputArchive>, int>>
auto BinaryFixture::CreateRegistered() const -> OutputArchive
{
    return OutputArchive("Test.dat", typeRegistrationContext);
}

template<class T, std::enable_if_t<std::is_same_v<T, BinaryFixture::InputArchive>, int>>
auto BinaryFixture::CreateRegistered() const -> InputArchive
{
    return InputArchive("Test.dat", typeRegistrationContext);
}