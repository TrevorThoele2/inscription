#pragma once

#include <unordered_map>
#include <Chroma/IsBracesConstructible.h>
#include "ContainerSize.h"

#include "OutputBinaryFormat.h"
#include "FilePositionScribe.h"

namespace Inscription
{
    template<class ID, class Object>
    class OutputJumpTable
    {
    public:
        void Save(File::OutputBinary& file, Format::Binary& format);

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
        struct JumpPoint
        {
            ID id;
            Object& object;

            File::Position jumpPosition = 0;
            JumpPoint(ID id, Object& object) : id(id), object(object)
            {}
        };

        void SaveJump(JumpPoint& point, File::OutputBinary& file, Format::Binary& format);
        void SaveObject(JumpPoint& point, File::OutputBinary& file, Format::Binary& format);
        void OverwriteJump(JumpPoint& point, File::Position overwriteWith, File::OutputBinary& file, Format::Binary& format);
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(
            ::Chroma::is_braces_default_constructible_v<ID>,
            "The ID used in an InputJumpTable must be default constructible.");
    };

    template<class ID, class Object>
    void OutputJumpTable<ID, Object>::Save(File::OutputBinary& file, Format::Binary& format)
    {
        std::vector<JumpPoint> jumpPoints;
        for (auto& loop : added)
            jumpPoints.push_back(JumpPoint(loop.first, loop.second.object));

        ContainerSize size(jumpPoints.size());
        format(size);

        const auto postSizePosition = file.Position();
        File::Position postTablePositionPlaceholder = 0;
        format(postTablePositionPlaceholder);

        for (auto& loop : jumpPoints)
            SaveJump(loop, file, format);

        for (auto& loop : jumpPoints)
            SaveObject(loop, file, format);

        File::Position postTablePosition = file.Position();
        file.Seek(postSizePosition);
        format(postTablePosition);
        file.Seek(postTablePosition);
    }

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
    void OutputJumpTable<ID, Object>::SaveJump(JumpPoint& point, File::OutputBinary& file, Format::Binary& format)
    {
        point.jumpPosition = file.Position();

        File::Position placeholder = 0;
        format(placeholder);
        format(point.id);
    }

    template<class ID, class Object>
    void OutputJumpTable<ID, Object>::SaveObject(
        JumpPoint& point, File::OutputBinary& file, Format::Binary& format)
    {
        const auto currentPosition = file.Position();
        format(point.object);
        OverwriteJump(point, currentPosition, file, format);
    }

    template<class ID, class Object>
    void OutputJumpTable<ID, Object>::OverwriteJump(
        JumpPoint& point, File::Position overwriteWith, File::OutputBinary& file, Format::Binary& format)
    {
        const auto currentPosition = file.Position();
        file.Seek(point.jumpPosition);
        format(overwriteWith);
        file.Seek(currentPosition);
    }
}
