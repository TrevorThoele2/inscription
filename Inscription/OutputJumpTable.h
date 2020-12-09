#pragma once

#include <unordered_map>
#include <Chroma/IsBracesConstructible.h>
#include "ContainerSize.h"

#include "CompositeScribeCategory.h"

namespace Inscription
{
    template<class ID, class Object>
    class OutputJumpTable
    {
    public:
        bool Add(ID id, Object& add);

        [[nodiscard]] std::vector<ID> AllIDs() const;
        [[nodiscard]] bool Contains(ID id) const;
        [[nodiscard]] size_t Size() const;
    private:
        struct Added
        {
            Object& object;

            explicit Added(Object& object);
        };

        std::unordered_map<ID, Added> added;
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(
            ::Chroma::is_braces_default_constructible_v<ID>,
            "The ID used in an InputJumpTable must be default constructible.");
    };

    template<class ID, class Object>
    bool OutputJumpTable<ID, Object>::Add(ID id, Object& add)
    {
        return added.emplace(id, Added{ add }).second;
    }

    template<class ID, class Object>
    auto OutputJumpTable<ID, Object>::AllIDs() const -> std::vector<ID>
    {
        std::vector<ID> returnValue;
        for (auto& loop : added)
            returnValue.push_back(loop.first);
        return returnValue;
    }

    template<class ID, class Object>
    bool OutputJumpTable<ID, Object>::Contains(ID id) const
    {
        for (auto& loop : added)
            if (loop.first == id)
                return true;

        return false;
    }

    template<class ID, class Object>
    size_t OutputJumpTable<ID, Object>::Size() const
    {
        return added.size();
    }

    template<class ID, class Object>
    OutputJumpTable<ID, Object>::Added::Added(Object& object) : object(object)
    {}

    template<class ID, class Object>
    class Scribe<OutputJumpTable<ID, Object>>
    {
    public:
        using ObjectT = OutputJumpTable<ID, Object>;
    private:
        using ArchivePosition = StreamPosition;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive)
        {
            std::vector<JumpPoint> jumpPoints;
            for (auto& loop : object.added)
                jumpPoints.push_back(JumpPoint(loop.first, loop.second.object));

            ContainerSize size(jumpPoints.size());
            archive(size);

            const auto postSizePosition = archive.CurrentStreamPosition();
            ArchivePosition postTablePositionPlaceholder = 0;
            archive(postTablePositionPlaceholder);

            for (auto& loop : jumpPoints)
                SaveJump(loop, archive);

            for (auto& loop : jumpPoints)
                SaveObject(loop, archive);

            ArchivePosition postTablePosition = archive.CurrentStreamPosition();
            archive.SeekStreamFromCurrent(postSizePosition);
            archive(postTablePosition);
            archive.SeekStreamFromCurrent(postTablePosition);
        }
    private:
        struct JumpPoint
        {
            ID id;
            Object& object;

            StreamPosition jumpPosition = 0;
            JumpPoint(ID id, Object& object) : id(id), object(object)
            {}
        };

        void SaveJump(JumpPoint& point, BinaryArchive& archive)
        {
            point.jumpPosition = archive.CurrentStreamPosition();

            ArchivePosition placeholder = 0;
            archive(placeholder);
            archive(point.id);
        }

        void SaveObject(JumpPoint& point, BinaryArchive& archive)
        {
            const auto currentPosition = archive.CurrentStreamPosition();
            archive(point.object);
            OverwriteJump(point, currentPosition, archive);
        }

        void OverwriteJump(JumpPoint& point, ArchivePosition overwriteWith, BinaryArchive& archive)
        {
            const auto currentPosition = archive.CurrentStreamPosition();
            archive.SeekStreamFromCurrent(point.jumpPosition);
            archive(overwriteWith);
            archive.SeekStreamFromCurrent(currentPosition);
        }
    };

    template<class ID, class Object>
    struct ScribeTraits<OutputJumpTable<ID, Object>, BinaryArchive> final
    {
        using Category = CompositeScribeCategory<OutputJumpTable<ID, Object>>;
    };
}
