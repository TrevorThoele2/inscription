#pragma once

#include <TestFramework/DataGeneration.h>

#include <Inscription/OutputTextArchive.h>
#include <Inscription/InputTextArchive.h>

#include <Inscription/Access.h>

class TextFixture
{
public:
    ::TestFramework::DataGeneration dataGeneration;

    using OutputArchive = ::Inscription::OutputTextArchive;
    using InputArchive = ::Inscription::InputTextArchive;

    OutputArchive CreateOutputArchive() const;
    InputArchive CreateInputArchive() const;
};

TextFixture::OutputArchive TextFixture::CreateOutputArchive() const
{
    return OutputArchive("Test.txt");
}

TextFixture::InputArchive TextFixture::CreateInputArchive() const
{
    return InputArchive("Test.txt");
}