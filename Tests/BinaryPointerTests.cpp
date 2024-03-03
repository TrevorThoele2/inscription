#include <catch.hpp>

#include <Inscription/TableScribe.h>
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

    struct NestedObject
    {
        int value = 0;
        NestedObject* next = nullptr;
    };

    class ProtectedConstructor
    {
    public:
        static ProtectedConstructor* Construct(int value)
        {
            return new ProtectedConstructor(value);
        }

        int Value() const
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

        int Value() const
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
    class Scribe<::BinaryPointerFixture::Object, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::Object, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::BinaryPointerFixture::NestedObject, BinaryArchive> : public
        CompositeScribe<::BinaryPointerFixture::NestedObject, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
            archive(object.next);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::BinaryPointerFixture::ProtectedConstructor, BinaryArchive> final :
        public CompositeScribe<::BinaryPointerFixture::ProtectedConstructor, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };

    template<>
    class Scribe<::BinaryPointerFixture::PrivateConstructor, BinaryArchive> final :
        public CompositeScribe<::BinaryPointerFixture::PrivateConstructor, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.value);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }
    };
}

TEST_CASE_METHOD(BinaryPointerFixture, "binary pointer")
{
    SECTION("null pointer saves and loads")
    {
        Object* saved = nullptr;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        Object* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<OutputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded == nullptr);
    }

    SECTION("loads when object is saved beforehand")
    {
        Object savedObject = dataGeneration.RandomStack<Object, int>();
        Object* savedPointer = &savedObject;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject);
            outputArchive(savedPointer);
        }

        Object loadedObject;
        Object* loadedPointer;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedObject);
            inputArchive(loadedPointer);
        }

        REQUIRE(loadedObject.value == savedObject.value);
        REQUIRE(loadedPointer == &loadedObject);
    }

    SECTION("loads nested object")
    {
        NestedObject* savedObject1 = dataGeneration.RandomHeap<NestedObject, int>();
        NestedObject* savedObject2 = dataGeneration.RandomHeap<NestedObject, int>();
        savedObject1->next = savedObject2;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject1);
        }

        NestedObject* loadedObject1 = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedObject1);
        }

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

    SECTION("loads cyclic object")
    {
        NestedObject* savedObject1 = dataGeneration.RandomHeap<NestedObject, int>();
        NestedObject* savedObject2 = dataGeneration.RandomHeap<NestedObject, int>();
        savedObject1->next = savedObject2;
        savedObject2->next = savedObject1;

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(savedObject1);
        }

        NestedObject* loadedObject1 = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loadedObject1);
        }

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

    SECTION("loads protected constructor defined object")
    {
        auto saved = ProtectedConstructor::Construct(dataGeneration.Random<int>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        ProtectedConstructor* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded != nullptr);
        REQUIRE(loaded->Value() == saved->Value());

        delete saved;
        delete loaded;
    }

    SECTION("loads private constructor defined object")
    {
        auto saved = PrivateConstructor::Construct(dataGeneration.Random<int>());

        {
            auto outputArchive = CreateRegistered<OutputArchive>();
            outputArchive(saved);
        }

        PrivateConstructor* loaded = nullptr;

        {
            auto inputArchive = CreateRegistered<InputArchive>();
            inputArchive(loaded);
        }

        REQUIRE(loaded != nullptr);
        REQUIRE(loaded->Value() == saved->Value());

        delete saved;
        delete loaded;
    }
}