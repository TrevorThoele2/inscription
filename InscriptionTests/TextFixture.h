#pragma once

#include "GeneralFixture.h"

#include <TestFramework/DataGeneration.h>

#include <Inscription/OutputTextArchive.h>
#include <Inscription/InputTextArchive.h>

#include <Inscription/Access.h>

class TextFixture : public GeneralFixture
{
public:
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