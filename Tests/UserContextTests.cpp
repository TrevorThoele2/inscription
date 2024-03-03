#include <catch.hpp>

#include "GeneralFixture.h"

#include "Inscription/Archive.h"

class ArchiveTestsFixture : public GeneralFixture
{
public:
    class DerivedArchive final : public Inscription::Archive::Archive
    {
        
    };
};

SCENARIO_METHOD(ArchiveTestsFixture, "user contexts")
{
    GIVEN("archive")
    {
        DerivedArchive archive;

        WHEN("retrieving user context")
        {
            auto retrieved = archive.UserContext<std::string>();

            THEN("is nullptr")
            {
                REQUIRE(retrieved == nullptr);
            }
        }

        WHEN("removing user context")
        {
            THEN("does not throw exception")
            {
                REQUIRE_NOTHROW(archive.RemoveUserContext<std::string>());
            }
        }

        WHEN("emplacing user context")
        {
            auto string = dataGeneration.Random<std::string>();

            archive.EmplaceUserContext(&string);

            THEN("retrieving that user context gives valid pointer")
            {
                auto retrieved = archive.UserContext<std::string>();

                REQUIRE(retrieved == &string);
            }

            THEN("retrieving other user context gives nullptr")
            {
                auto retrieved = archive.UserContext<int>();

                REQUIRE(retrieved == nullptr);
            }

            WHEN("removing user context")
            {
                archive.RemoveUserContext<std::string>();

                THEN("retrieving user context in old archive gives nullptr")
                {
                    auto retrieved = archive.UserContext<std::string>();

                    REQUIRE(retrieved == nullptr);
                }
            }

            WHEN("move constructing archive")
            {
                auto newArchive(std::move(archive));

                WHEN("retrieving from new archive")
                {
                    THEN("retrieving user context in old archive gives valid pointer")
                    {
                        auto retrieved = newArchive.UserContext<std::string>();

                        REQUIRE(retrieved == &string);
                    }

                    THEN("retrieving other user context gives nullptr")
                    {
                        auto retrieved = newArchive.UserContext<int>();

                        REQUIRE(retrieved == nullptr);
                    }
                }
            }

            WHEN("move assigning archive")
            {
                auto newArchive = std::move(archive);

                WHEN("retrieving from new archive")
                {
                    THEN("retrieving user context in old archive gives valid pointer")
                    {
                        auto retrieved = newArchive.UserContext<std::string>();

                        REQUIRE(retrieved == &string);
                    }

                    THEN("retrieving other user context gives nullptr")
                    {
                        auto retrieved = newArchive.UserContext<int>();

                        REQUIRE(retrieved == nullptr);
                    }
                }
            }
        }
    }
}