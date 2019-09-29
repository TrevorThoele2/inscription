#include <catch.hpp>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/CompositeScribe.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>
#include <Inscription/TupleScribe.h>
#include <Inscription/StreamPositionScribe.h>

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

#include "BinaryFixture.h"

class BinaryIntegrationTestsFixture : public BinaryFixture
{
public:
    BinaryIntegrationTestsFixture()
    {
        typeRegistrationContext.RegisterType<TestClass>();
    }

    class TestClass
    {
    public:
        explicit TestClass(int value = 0) : value(value)
        {}

        [[nodiscard]] int Value() const
        {
            return value;
        }
    private:
        int value;
    private:
        INSCRIPTION_ACCESS;
    };
};

namespace Inscription
{
    template<>
    class Scribe<::BinaryIntegrationTestsFixture::TestClass, BinaryArchive> final :
        public CompositeScribe<::BinaryIntegrationTestsFixture::TestClass, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
        }
    };
}

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

        auto savedObject = dataGeneration.RandomStack<TestClass, int>();
        auto savedPointer = &savedObject;
        auto savedUniquePtr = std::unique_ptr<TestClass>(dataGeneration.RandomHeap<TestClass, int>());

        auto savedString = dataGeneration.Random<std::string>();

        auto savedTuple = dataGeneration.RandomStack<std::tuple<int, short, std::string>, int, short, std::string>();

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
            outputArchive(savedInt8);
            outputArchive(savedInt16);
            outputArchive(savedInt32);
            outputArchive(savedInt64);

            outputArchive(savedUint8);
            outputArchive(savedUint16);
            outputArchive(savedUint32);
            outputArchive(savedUint64);

            outputArchive(savedFloat);
            outputArchive(savedDouble);

            outputArchive(savedObject);
            outputArchive(savedPointer);
            outputArchive(savedUniquePtr);

            outputArchive(savedString);

            outputArchive(savedTuple);

            outputArchive(savedArray);
            outputArchive(savedForwardList);
            outputArchive(savedList);
            outputArchive(savedMap);
            outputArchive(savedMultiMap);
            outputArchive(savedMultiSet);
            outputArchive(savedQueue);
            outputArchive(savedSet);
            outputArchive(savedStack);
            outputArchive(savedUnorderedMap);
            outputArchive(savedUnorderedMultiMap);
            outputArchive(savedUnorderedMultiSet);
            outputArchive(savedUnorderedSet);
            outputArchive(savedVector);

            outputArchive(savedStreamPosition);
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

            Inscription::StreamPosition loadedStreamPosition;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedInt8);
                inputArchive(loadedInt16);
                inputArchive(loadedInt32);
                inputArchive(loadedInt64);

                inputArchive(loadedUint8);
                inputArchive(loadedUint16);
                inputArchive(loadedUint32);
                inputArchive(loadedUint64);

                inputArchive(loadedFloat);
                inputArchive(loadedDouble);

                inputArchive(loadedObject);
                inputArchive(loadedPointer);
                inputArchive(loadedUniquePtr);

                inputArchive(loadedString);

                inputArchive(loadedTuple);

                inputArchive(loadedArray);
                inputArchive(loadedForwardList);
                inputArchive(loadedList);
                inputArchive(loadedMap);
                inputArchive(loadedMultiMap);
                inputArchive(loadedMultiSet);
                inputArchive(loadedQueue);
                inputArchive(loadedSet);
                inputArchive(loadedStack);
                inputArchive(loadedUnorderedMap);
                inputArchive(loadedUnorderedMultiMap);
                inputArchive(loadedUnorderedMultiSet);
                inputArchive(loadedUnorderedSet);
                inputArchive(loadedVector);

                inputArchive(loadedStreamPosition);
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

                REQUIRE(loadedObject.Value() == savedObject.Value());
                REQUIRE(loadedPointer->Value() == savedPointer->Value());
                REQUIRE(loadedUniquePtr->Value() == savedUniquePtr->Value());

                REQUIRE(loadedString == savedString);

                REQUIRE(loadedTuple == savedTuple);

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