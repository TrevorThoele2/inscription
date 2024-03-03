#pragma once

#include <memory>

#include "Scribe.h"
#include "ScribeBase.h"

namespace Inscription
{
    template<class Object, class Archive>
    class NumericScribe : public ScribeBase<Object, Archive>
    {
    private:
        using BaseT = ScribeBase<Object, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;

        using BaseT::DoBasicConstruction;
    private:
        constexpr static bool isEnum = std::is_enum_v<ObjectT>;
    private:
        static_assert(std::is_arithmetic_v<ObjectT> || std::is_enum_v<ObjectT>,
            "The Object given to a NumericScribe was not arithmetic.");
    };

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::ScrivenImplementation(ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
            archive.AsOutput()->Write(object);
        else
            archive.AsInput()->Read(object);
    }

    template<class Object, class Archive>
    void NumericScribe<Object, Archive>::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }

    template<class Archive>
    class Scribe<bool, Archive> : public NumericScribe<bool, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint8_t, Archive> : public NumericScribe<std::uint8_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint16_t, Archive> : public NumericScribe<std::uint16_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint32_t, Archive> : public NumericScribe<std::uint32_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::uint64_t, Archive> : public NumericScribe<std::uint64_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int8_t, Archive> : public NumericScribe<std::int8_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int16_t, Archive> : public NumericScribe<std::int16_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int32_t, Archive> : public NumericScribe<std::int32_t, Archive>
    {};

    template<class Archive>
    class Scribe<std::int64_t, Archive> : public NumericScribe<std::int64_t, Archive>
    {};

    template<class Archive>
    class Scribe<char, Archive> : public NumericScribe<char, Archive>
    {};

    template<class Archive>
    class Scribe<float, Archive> : public NumericScribe<float, Archive>
    {};

    template<class Archive>
    class Scribe<double, Archive> : public NumericScribe<double, Archive>
    {};

    template<>
    struct ObjectTrackingTraits<bool, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::uint8_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::uint16_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::uint32_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::uint64_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::int8_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::int16_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::int32_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<std::int64_t, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<float, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };

    template<>
    struct ObjectTrackingTraits<double, BinaryArchive>
    {
        static constexpr bool shouldTrack = false;
    };
}