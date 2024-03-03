#include <catch.hpp>

#include "GeneralFixture.h"

#include <Inscription/Format.h>

class UserContextTests : public GeneralFixture
{
public:
    class DerivedFormat final : public Inscription::Format::Format
    {
        
    };
};

SCENARIO_METHOD(UserContextTests, "user contexts")
{
    GIVEN("format")
    {
        DerivedFormat format;

        WHEN("retrieving user context")
        {
            auto retrieved = format.UserContext<std::string>();

            THEN("is nullptr")
            {
                REQUIRE(retrieved == nullptr);
            }
        }

        WHEN("removing user context")
        {
            THEN("does not throw exception")
            {
                REQUIRE_NOTHROW(format.RemoveUserContext<std::string>());
            }
        }

        WHEN("emplacing user context")
        {
            auto string = dataGeneration.Random<std::string>();

            format.EmplaceUserContext(&string);

            THEN("retrieving that user context gives valid pointer")
            {
                auto retrieved = format.UserContext<std::string>();

                REQUIRE(retrieved == &string);
            }

            THEN("retrieving other user context gives nullptr")
            {
                auto retrieved = format.UserContext<int>();

                REQUIRE(retrieved == nullptr);
            }

            WHEN("removing user context")
            {
                format.RemoveUserContext<std::string>();

                THEN("retrieving user context in old format gives nullptr")
                {
                    auto retrieved = format.UserContext<std::string>();

                    REQUIRE(retrieved == nullptr);
                }
            }

            WHEN("move constructing format")
            {
                auto newFormat(std::move(format));

                WHEN("retrieving from new format")
                {
                    THEN("retrieving user context in old format gives valid pointer")
                    {
                        auto retrieved = newFormat.UserContext<std::string>();

                        REQUIRE(retrieved == &string);
                    }

                    THEN("retrieving other user context gives nullptr")
                    {
                        auto retrieved = newFormat.UserContext<int>();

                        REQUIRE(retrieved == nullptr);
                    }
                }
            }

            WHEN("move assigning format")
            {
                auto newFormat = std::move(format);

                WHEN("retrieving from new format")
                {
                    THEN("retrieving user context in old format gives valid pointer")
                    {
                        auto retrieved = newFormat.UserContext<std::string>();

                        REQUIRE(retrieved == &string);
                    }

                    THEN("retrieving other user context gives nullptr")
                    {
                        auto retrieved = newFormat.UserContext<int>();

                        REQUIRE(retrieved == nullptr);
                    }
                }
            }
        }
    }
}