#pragma once

#include <TestFramework/DataGeneration.h>
#include <Inscription/TypeRegistrationContext.h>

#include <Inscription/OutputBinaryScribe.h>
#include <Inscription/InputBinaryScribe.h>
#include <Inscription/BinaryConvenience.h>
#include <Inscription/InscripterBase.h>

class BinaryFixture
{
public:
    ::TestFramework::DataGeneration dataGeneration;

    typedef ::Inscription::TypeRegistrationContext<::Inscription::BinaryScribe> TypeRegistrationContext;
    TypeRegistrationContext typeRegistrationContext;

    typedef ::Inscription::OutputBinaryScribe OutputScribe;
    typedef ::Inscription::InputBinaryScribe InputScribe;

    template<class T>
    T CreateRegistered() const;
    template<>
    OutputScribe CreateRegistered() const;
    template<>
    InputScribe CreateRegistered() const;
};

template<class T>
T BinaryFixture::CreateRegistered() const
{
    static_assert(false, "A generic registered type cannot be created. Use one of the explicit overloads.");
}

template<>
BinaryFixture::OutputScribe BinaryFixture::CreateRegistered() const
{
    return OutputScribe("Test.dat", "testing", 1, typeRegistrationContext);
}

template<>
BinaryFixture::InputScribe BinaryFixture::CreateRegistered() const
{
    return InputScribe("Test.dat", "testing", typeRegistrationContext);
}