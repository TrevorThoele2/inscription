#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/TupleScribe.h>
#include <Inscription/StreamPositionScribe.h>
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

#include "JsonFixture.h"

#include "TestClass.h"

class JsonIntegrationTestsFixture : public JsonFixture
{
public:
    JsonIntegrationTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }
};

SCENARIO_METHOD(JsonIntegrationTestsFixture, "loading every type in json", "[json][integration]")
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

        auto savedStreamPosition = Inscription::StreamPosition(dataGeneration.Random<unsigned long long>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive("int8", savedInt8);
            outputArchive("int16", savedInt16);
            outputArchive("int32", savedInt32);
            outputArchive("int64", savedInt64);

            outputArchive("uint8", savedUint8);
            outputArchive("uint16", savedUint16);
            outputArchive("uint32", savedUint32);
            outputArchive("uint64", savedUint64);

            outputArchive("float", savedFloat);
            outputArchive("double", savedDouble);

            outputArchive("object", savedObject);

            outputArchive("string", savedString);

            outputArchive("array", savedArray);
            outputArchive("forward_list", savedForwardList);
            outputArchive("list", savedList);
            outputArchive("map", savedMap);
            outputArchive("multimap", savedMultiMap);
            outputArchive("multiset", savedMultiSet);
            outputArchive("queue", savedQueue);
            outputArchive("set", savedSet);
            outputArchive("stack", savedStack);
            outputArchive("unordered_map", savedUnorderedMap);
            outputArchive("unordered_multimap", savedUnorderedMultiMap);
            outputArchive("unordered_multiset", savedUnorderedMultiSet);
            outputArchive("unordered_set", savedUnorderedSet);
            outputArchive("vector", savedVector);
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

            std::string loadedString;

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

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive("int8", loadedInt8);
                inputArchive("int16", loadedInt16);
                inputArchive("int32", loadedInt32);
                inputArchive("int64", loadedInt64);

                inputArchive("uint8", loadedUint8);
                inputArchive("uint16", loadedUint16);
                inputArchive("uint32", loadedUint32);
                inputArchive("uint64", loadedUint64);

                inputArchive("float", loadedFloat);
                inputArchive("double", loadedDouble);

                inputArchive("object", loadedObject);

                inputArchive("string", loadedString);

                inputArchive("array", loadedArray);
                inputArchive("forward_list", loadedForwardList);
                inputArchive("list", loadedList);
                inputArchive("map", loadedMap);
                inputArchive("multimap", loadedMultiMap);
                inputArchive("multiset", loadedMultiSet);
                inputArchive("queue", loadedQueue);
                inputArchive("set", loadedSet);
                inputArchive("stack", loadedStack);
                inputArchive("unordered_map", loadedUnorderedMap);
                inputArchive("unordered_multimap", loadedUnorderedMultiMap);
                inputArchive("unordered_multiset", loadedUnorderedMultiSet);
                inputArchive("unordered_set", loadedUnorderedSet);
                inputArchive("vector", loadedVector);
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

                REQUIRE(loadedFloat == Approx(savedFloat));
                REQUIRE(loadedDouble == Approx(savedDouble));

                REQUIRE(loadedObject == savedObject);

                REQUIRE(loadedString == savedString);

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
            }
        }
    }
}