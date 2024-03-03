#pragma once

#include <unordered_map>
#include <Chroma/IsBracesConstructible.h>
#include "ContainerSize.h"

#include "CompositeScribe.h"

namespace Inscription
{
    template<class ID, class Object>
    class OutputJumpTable
    {
    public:
        bool Add(ID id, Object& add);

        [[nodiscard]] std::vector<ID> AllIDs() const;
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
    OutputJumpTable<ID, Object>::Added::Added(Object& object) : object(object)
    {}

    template<class ID, class Object, class Archive>
    class Scribe<OutputJumpTable<ID, Object>, Archive> :
        public CompositeScribe<OutputJumpTable<ID, Object>, Archive>
    {
    private:
        using BaseT = CompositeScribe<OutputJumpTable<ID, Object>, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    private:
        using ArchivePosition = typename ArchiveT::StreamPosition;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            std::vector<JumpPoint> jumpPoints;
            for (auto& loop : object.added)
                jumpPoints.push_back(JumpPoint(loop.first, loop.second.object));

            ContainerSize size(jumpPoints.size());
            archive(size);

            ArchivePosition postSizePosition = archive.TellStream();
            ArchivePosition postTablePositionPlaceholder = 0;
            archive(postTablePositionPlaceholder);

            for (auto& loop : jumpPoints)
                SaveJump(loop, archive);

            for (auto& loop : jumpPoints)
                SaveObject(loop, archive);

            ArchivePosition postTablePosition = archive.TellStream();
            archive.SeekStream(postSizePosition);
            archive(postTablePosition);
            archive.SeekStream(postTablePosition);
        }
    private:
        using StreamPosition = typename ArchiveT::StreamPosition;

        struct JumpPoint
        {
            ID id;
            Object& object;

            StreamPosition jumpPosition = 0;
            JumpPoint(ID id, Object& object) : id(id), object(object)
            {}
        };

        void SaveJump(JumpPoint& point, ArchiveT& archive)
        {
            point.jumpPosition = archive.TellStream();

            ArchivePosition placeholder = 0;
            archive(placeholder);
            archive(point.id);
        }

        void SaveObject(JumpPoint& point, ArchiveT& archive)
        {
            ArchivePosition currentPosition = archive.TellStream();
            archive(point.object);
            OverwriteJump(point, currentPosition, archive);
        }

        void OverwriteJump(JumpPoint& point, ArchivePosition overwriteWith, ArchiveT& archive)
        {
            ArchivePosition currentPosition = archive.TellStream();
            archive.SeekStream(point.jumpPosition);
            archive(overwriteWith);
            archive.SeekStream(currentPosition);
        }
    };
}
