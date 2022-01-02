#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/TupleScribe.h>
#include <Inscription/FilePositionScribe.h>
#include <Inscription/VariantScribe.h>

#include <Inscription/ArrayScribe.h>
#include <Inscription/ForwardListScribe.h>
#include <Inscription/ListScribe.h>
#include <Inscription/MapScribe.h>
#include <Inscription/MultimapScribe.h>
#include <Inscription/MultisetScribe.h>
#include <Inscription/QueueScribe.h>
#include <Inscription/SetScribe.h>
#include <Inscription/StackScribe.h>
#include <Inscription/UnorderedMapScribe.h>
#include <Inscription/UnorderedMultimapScribe.h>
#include <Inscription/UnorderedMultisetScribe.h>
#include <Inscription/UnorderedSetScribe.h>
#include <Inscription/VectorScribe.h>

#include "GeneralFixture.h"

#include "TestClass.h"

class BinaryIntegrationTestsFixture : public GeneralFixture
{
public:
    Inscription::TypeRegistrationContext<Inscription::Format::Binary> typeRegistrationContext;

    BinaryIntegrationTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(BinaryIntegrationTestsFixture, "loading every type in binary", "[binary][integration]")
{
    GIVEN("every type saved")
    {
        auto savedInt8 = dataGeneration.Random<std::int8_t>();
        auto savedInt16 = dataGeneration.Random<std::int16_t>();
        auto savedInt32 = dataGeneration.Random<std::int32_t>();
        auto savedInt64 = dataGeneration.Random<std::int64_t>();

        auto savedUint8 = dataGeneration.Random<std::uint8_t>();
        auto savedUint16 = dataGeneration.Random<std::uint16_t>();
        auto savedUint32 = dataGeneration.Random<std::uint32_t>();
        auto savedUint64 = dataGeneration.Random<std::uint64_t>();

        auto savedFloat = dataGeneration.Random<float>();
        auto savedDouble = dataGeneration.Random<double>();

        auto savedObject = dataGeneration.RandomStack<TestClass, int, std::string>();
        auto savedPointer = &savedObject;
        auto savedUniquePtr = std::unique_ptr<TestClass>(dataGeneration.RandomHeap<TestClass, int, std::string>());

        auto savedString = dataGeneration.Random<std::string>();

        auto savedTuple = dataGeneration.RandomStack<std::tuple<int, short, std::string>, int, short, std::string>();

        auto savedVariant = dataGeneration.RandomStack<std::variant<int, short, std::string>, std::string>();

        auto savedArray = std::array<int, 3>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedForwardList = std::forward_list<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedList = std::list<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedMap = std::map<int, int>
        {
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() }
        };

        auto savedMultiMap = std::multimap<int, int>
        {
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() }
        };

        auto savedMultiSet = std::multiset<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedQueue = std::queue<int>();
        savedQueue.push(dataGeneration.Random<int>());
        savedQueue.push(dataGeneration.Random<int>());
        savedQueue.push(dataGeneration.Random<int>());

        auto savedSet = std::set<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedStack = std::stack<int>();
        savedStack.push(dataGeneration.Random<int>());
        savedStack.push(dataGeneration.Random<int>());
        savedStack.push(dataGeneration.Random<int>());

        auto savedUnorderedMap = std::unordered_map<int, int>
        {
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() }
        };

        auto savedUnorderedMultiMap = std::unordered_multimap<int, int>
        {
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() },
            { dataGeneration.Random<int>(), dataGeneration.Random<int>() }
        };

        auto savedUnorderedMultiSet = std::unordered_multiset<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedUnorderedSet = std::unordered_set<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedVector = std::vector<int>
        {
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>(),
            dataGeneration.Random<int>()
        };

        auto savedStreamPosition = Inscription::File::Position(dataGeneration.Random<unsigned long long>());

        {
            auto file = Inscription::File::OutputBinary("Test.dat");
            auto archive = Inscription::Archive::OutputBinary(file);
            auto format = Inscription::Format::OutputBinary(archive, typeRegistrationContext);
            
            format(savedInt8);
            format(savedInt16);
            format(savedInt32);
            format(savedInt64);

            format(savedUint8);
            format(savedUint16);
            format(savedUint32);
            format(savedUint64);

            format(savedFloat);
            format(savedDouble);

            format(savedObject);
            format(savedPointer);
            format(savedUniquePtr);

            format(savedString);

            format(savedTuple);

            format(savedVariant);

            format(savedArray);
            format(savedForwardList);
            format(savedList);
            format(savedMap);
            format(savedMultiMap);
            format(savedMultiSet);
            format(savedQueue);
            format(savedSet);
            format(savedStack);
            format(savedUnorderedMap);
            format(savedUnorderedMultiMap);
            format(savedUnorderedMultiSet);
            format(savedUnorderedSet);
            format(savedVector);

            format(savedStreamPosition);
        }

        WHEN("loading every type")
        {
            std::int8_t loadedInt8 = 0;
            std::int16_t loadedInt16 = 0;
            auto loadedInt32 = 0;
            std::int64_t loadedInt64 = 0;

            std::uint8_t loadedUint8 = 0;
            std::uint16_t loadedUint16 = 0;
            std::uint32_t loadedUint32 = 0;
            std::uint64_t loadedUint64 = 0;

            float loadedFloat = 0;
            double loadedDouble = 0;

            TestClass loadedObject;
            TestClass* loadedPointer = nullptr;
            std::unique_ptr<TestClass> loadedUniquePtr;

            std::string loadedString;

            std::tuple<int, short, std::string> loadedTuple;

            std::variant<int, short, std::string> loadedVariant;

            std::array<int, 3> loadedArray{ 0, 0, 0 };
            std::forward_list<int> loadedForwardList;
            std::list<int> loadedList;
            std::map<int, int> loadedMap;
            std::multimap<int, int> loadedMultiMap;
            std::multiset<int> loadedMultiSet;
            std::queue<int> loadedQueue;
            std::set<int> loadedSet;
            std::stack<int> loadedStack;
            std::unordered_map<int, int> loadedUnorderedMap;
            std::unordered_multimap<int, int> loadedUnorderedMultiMap;
            std::unordered_multiset<int> loadedUnorderedMultiSet;
            std::unordered_set<int> loadedUnorderedSet;
            std::vector<int> loadedVector;

            Inscription::File::Position loadedStreamPosition;

            {
                auto file = Inscription::File::InputBinary("Test.dat");
                auto archive = Inscription::Archive::InputBinary(file);
                auto format = Inscription::Format::InputBinary(archive, typeRegistrationContext);
                
                format(loadedInt8);
                format(loadedInt16);
                format(loadedInt32);
                format(loadedInt64);

                format(loadedUint8);
                format(loadedUint16);
                format(loadedUint32);
                format(loadedUint64);

                format(loadedFloat);
                format(loadedDouble);

                format(loadedObject);
                format(loadedPointer);
                format(loadedUniquePtr);

                format(loadedString);

                format(loadedTuple);

                format(loadedVariant);

                format(loadedArray);
                format(loadedForwardList);
                format(loadedList);
                format(loadedMap);
                format(loadedMultiMap);
                format(loadedMultiSet);
                format(loadedQueue);
                format(loadedSet);
                format(loadedStack);
                format(loadedUnorderedMap);
                format(loadedUnorderedMultiMap);
                format(loadedUnorderedMultiSet);
                format(loadedUnorderedSet);
                format(loadedVector);

                format(loadedStreamPosition);
            }

            THEN("every loaded type is same as saved type")
            {
                REQUIRE(loadedInt8 == savedInt8);
                REQUIRE(loadedInt16 == savedInt16);
                REQUIRE(loadedInt32 == savedInt32);
                REQUIRE(loadedInt64 == savedInt64);

                REQUIRE(loadedUint8 == savedUint8);
                REQUIRE(loadedUint16 == savedUint16);
                REQUIRE(loadedUint32 == savedUint32);
                REQUIRE(loadedUint64 == savedUint64);

                REQUIRE(loadedFloat == savedFloat);
                REQUIRE(loadedDouble == savedDouble);

                REQUIRE(loadedObject == savedObject);
                REQUIRE(loadedPointer->integer == savedPointer->integer);
                REQUIRE(loadedPointer->string == savedPointer->string);
                REQUIRE(loadedUniquePtr->integer == savedUniquePtr->integer);
                REQUIRE(loadedUniquePtr->string == savedUniquePtr->string);

                REQUIRE(loadedString == savedString);

                REQUIRE(loadedTuple == savedTuple);

                REQUIRE(loadedVariant == savedVariant);

                REQUIRE(loadedArray == savedArray);
                REQUIRE(loadedForwardList == savedForwardList);
                REQUIRE(loadedList == savedList);
                REQUIRE(loadedMap == savedMap);
                REQUIRE(loadedMultiMap == savedMultiMap);
                REQUIRE(loadedMultiSet == savedMultiSet);
                REQUIRE(loadedQueue == savedQueue);
                REQUIRE(loadedSet == savedSet);
                REQUIRE(loadedStack == savedStack);
                REQUIRE(loadedUnorderedMap == savedUnorderedMap);
                REQUIRE(loadedUnorderedMultiMap == savedUnorderedMultiMap);
                REQUIRE(loadedUnorderedMultiSet == savedUnorderedMultiSet);
                REQUIRE(loadedUnorderedSet == savedUnorderedSet);
                REQUIRE(loadedVector == savedVector);

                REQUIRE(loadedStreamPosition == savedStreamPosition);
            }
        }
    }
}