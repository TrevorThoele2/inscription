#pragma once

#include "GeneralFixture.h"

#include <Inscription/TypeRegistrationContext.h>

#include <Inscription/OutputBinaryArchive.h>
#include <Inscription/InputBinaryArchive.h>

#include <Inscription/Access.h>

class BinaryFixture : public GeneralFixture
{
public:
    using TypeRegistrationContext = ::Inscription::TypeRegistrationContext<::Inscription::BinaryArchive>;
    TypeRegistrationContext typeRegistrationContext;

    using OutputArchive = ::Inscription::OutputBinaryArchive;
    using InputArchive = ::Inscription::InputBinaryArchive;

    template<class T>
    T CreateRegistered() const;
    template<>
    OutputArchive CreateRegistered() const;
    template<>
    InputArchive CreateRegistered() const;
};

template<class T>
T BinaryFixture::CreateRegistered() const
{
    static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
}

template<>
BinaryFixture::OutputArchive BinaryFixture::CreateRegistered() const
{
    return OutputArchive("Test.dat", "testing", 1, typeRegistrationContext);
}

template<>
BinaryFixture::InputArchive BinaryFixture::CreateRegistered() const
{
    return InputArchive("Test.dat", "testing", typeRegistrationContext);
}