#include <catch.hpp>

#include <Inscription/CompositeScribeCategory.h>
#include <Inscription/TableScribeCategory.h>

#include <Inscription/NumericScribe.h>
#include <Inscription/PointerScribe.h>
#include <Inscription/MemoryScribe.h>
#include <Inscription/StringScribe.h>

#include "BinaryFixture.h"

class BinaryPointerFixture : public BinaryFixture
{
public:
    BinaryPointerFixture()
    {
        typeRegistrationContext.RegisterType<Object>();
    }

    struct Object
    {
        int value = 0;
    };

    struct NestedHeapObject
    {
        int value = 0;
        NestedHeapObject* next = nullptr;
    };

    class ProtectedConstructor
    {
    public:
        static ProtectedConstructor* Construct(int value)
        {
            return new ProtectedConstructor(value);
        }

        [[nodiscard]] int Value() const
        {
            return value;
        }
    protected:
        explicit ProtectedConstructor(int value = 0) : value(value)
        {}
    private:
        int value;
    private:
        INSCRIPTION_ACCESS;
    };

    class PrivateConstructor
    {
    public:
        static PrivateConstructor* Construct(int value)
        {
            return new PrivateConstructor(value);
        }

        [[nodiscard]] int Value() const
        {
            return value;
        }
    private:
        explicit PrivateConstructor(int value = 0) : value(value)
        {}
    private:
        int value;
    private:
        INSCRIPTION_ACCESS;
    };
};

namespace Inscription
{
    template<>
    class Scribe<BinaryPointerFixture::Object>
    {
    public:
        using ObjectT = BinaryPointerFixture::Object;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive)
        {
            archive(object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<BinaryPointerFixture::Object, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPointerFixture::Object>;
    };

    template<>
    class Scribe<BinaryPointerFixture::NestedHeapObject>
    {
    public:
        using ObjectT = BinaryPointerFixture::NestedHeapObject;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive)
        {
            archive(object.value);
            archive(object.next);
        }
    };

    template<class Archive>
    struct ScribeTraits<BinaryPointerFixture::NestedHeapObject, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPointerFixture::NestedHeapObject>;
    };

    template<>
    class Scribe<BinaryPointerFixture::ProtectedConstructor> final
    {
    public:
        using ObjectT = BinaryPointerFixture::ProtectedConstructor;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive)
        {
            archive(object.value);
        }
    };

    template<class Archive>
    struct ScribeTraits<BinaryPointerFixture::ProtectedConstructor, Archive> final
    {
        using Category = CompositeScribeCategory<BinaryPointerFixture::ProtectedConstructor>;
    };

    template<>
    class Scribe<BinaryPointerFixture::PrivateConstructor> final
    {
    public:
        using ObjectT = BinaryPointerFixture::PrivateConstructor;
    public:
        void Scriven(ObjectT& object, Archive::Binary& archive)
        {
            archive(object.value);
        }
    };
}

SCENARIO_METHOD(BinaryPointerFixture, "loading null pointer in binary", "[binary][pointer]")
{
    GIVEN("saved null pointer")
    {
        Object* saved = nullptr;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading pointer")
        {
            Object* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<OutputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded pointer is null")
            {
                REQUIRE(loaded == nullptr);
            }
        }
    }
}

SCENARIO_METHOD(BinaryPointerFixture, "loading object before pointer in binary", "[binary][pointer]")
{
    GIVEN("object saved before pointer")
    {
        Object savedObject = dataGeneration.RandomStack<Object, int>();
        Object* savedPointer = &savedObject;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject);
            outputArchive(savedPointer);
        }

        WHEN("loading object then pointer")
        {
            Object loadedObject;
            Object* loadedPointer;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedObject);
                inputArchive(loadedPointer);
            }

            THEN("loaded pointer points to loaded object")
            {
                REQUIRE(loadedObject.value == savedObject.value);
                REQUIRE(loadedPointer == &loadedObject);
            }
        }
    }
}

SCENARIO_METHOD(BinaryPointerFixture, "loading nested object in binary", "[binary][pointer]")
{
    GIVEN("saved nested object")
    {
        NestedHeapObject* savedObject1 = dataGeneration.RandomHeap<NestedHeapObject, int>();
        NestedHeapObject* savedObject2 = dataGeneration.RandomHeap<NestedHeapObject, int>();
        savedObject1->next = savedObject2;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject1);
        }

        WHEN("loading nested object")
        {
            NestedHeapObject* loadedObject1 = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedObject1);
            }

            THEN("loaded nested object is valid")
            {
                REQUIRE(loadedObject1 != nullptr);
                REQUIRE(loadedObject1->next != nullptr);

                auto loadedObject2 = loadedObject1->next;

                REQUIRE(loadedObject1->next == loadedObject2);
                REQUIRE(loadedObject2->next == nullptr);
                REQUIRE(loadedObject1->value == savedObject1->value);
                REQUIRE(loadedObject2->value == savedObject2->value);

                delete savedObject1;
                delete savedObject2;
                delete loadedObject1;
                delete loadedObject2;
            }
        }
    }
}

SCENARIO_METHOD(BinaryPointerFixture, "loading cyclic heap objects in binary", "[binary][pointer]")
{
    GIVEN("saved cyclic object")
    {
        NestedHeapObject* savedObject1 = dataGeneration.RandomHeap<NestedHeapObject, int>();
        NestedHeapObject* savedObject2 = dataGeneration.RandomHeap<NestedHeapObject, int>();
        savedObject1->next = savedObject2;
        savedObject2->next = savedObject1;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject1);
        }

        WHEN("loading cyclic object")
        {
            NestedHeapObject* loadedObject1 = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loadedObject1);
            }

            THEN("loaded cyclic object is valid")
            {
                REQUIRE(loadedObject1 != nullptr);
                REQUIRE(loadedObject1->next != nullptr);

                auto loadedObject2 = loadedObject1->next;

                REQUIRE(loadedObject1->next == loadedObject2);
                REQUIRE(loadedObject2->next == loadedObject1);
                REQUIRE(loadedObject1->value == savedObject1->value);
                REQUIRE(loadedObject2->value == savedObject2->value);

                delete savedObject1;
                delete savedObject2;
                delete loadedObject1;
                delete loadedObject2;
            }
        }
    }
}

SCENARIO_METHOD(BinaryPointerFixture, "loading non-public constructor object pointers in binary", "[binary][pointer]")
{
    GIVEN("saved protected constructor object")
    {
        auto saved = ProtectedConstructor::Construct(dataGeneration.Random<int>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading protected constructor object")
        {
            ProtectedConstructor* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded protected constructor object is valid")
            {
                REQUIRE(loaded != nullptr);
                REQUIRE(loaded->Value() == saved->Value());

                delete saved;
                delete loaded;
            }
        }
    }

    GIVEN("saved private constructor object")
    {
        auto saved = PrivateConstructor::Construct(dataGeneration.Random<int>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        WHEN("loading private constructor object")
        {
            PrivateConstructor* loaded = nullptr;

            {
                auto inputArchive = CreateRegistered<InputArchive>();
                inputArchive(loaded);
            }

            THEN("loaded private constructor object is valid")
            {
                REQUIRE(loaded != nullptr);
                REQUIRE(loaded->Value() == saved->Value());

                delete saved;
                delete loaded;
            }
        }
    }
}