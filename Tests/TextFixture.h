#pragma once

#include "GeneralFixture.h"

#include <TestFramework/DataGeneration.h>

#include <Inscription/OutputTextArchive.h>
#include <Inscription/InputTextArchive.h>

#include <Inscription/Access.h>

class TextFixture : public GeneralFixture
{
public:
    using OutputArchive = Inscription::OutputTextArchive;
    using InputArchive = Inscription::InputTextArchive;

    [[nodiscard]] OutputArchive CreateOutputArchive() const;
    [[nodiscard]] InputArchive CreateInputArchive() const;
};

auto TextFixture::CreateOutputArchive() const -> OutputArchive
{
    return OutputArchive("Test.txt");
}

auto TextFixture::CreateInputArchive() const -> InputArchive
{
    return InputArchive("Test.txt");
}