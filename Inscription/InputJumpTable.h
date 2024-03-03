#pragma once

#include <vector>
#include <algorithm>
#include <Chroma/IsBracesConstructible.h>

#include "ContainerSize.h"

#include "StreamPosition.h"
#include "CompositeScribeCategory.h"
#include "StreamPositionScribe.h"

namespace Inscription
{
    template<class ID, class Object>
    class InputJumpTable
    {
    public:
        template<class Archive>
        bool FillObject(ID at, Object& object, Archive& archive);

        [[nodiscard]] std::vector<ID> AllIDs() const;
        [[nodiscard]] bool Contains(ID id) const;
        [[nodiscard]] size_t Size() const;
    private:
        struct Handle
        {
            ID id;
            StreamPosition jumpPosition;
        };

        using Handles = std::vector<Handle>;
        Handles handles;

        using iterator = typename Handles::iterator;
    private:
        template<class Archive>
        void LoadObject(Object& object, iterator at, Archive& archive);
        template<class Archive, class Function>
        void LoadObject(Function function, Object& object, iterator at, Archive& archive);
        iterator FindHandle(ID at);
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(
            ::Chroma::is_braces_default_constructible_v<ID>,
            "The ID used in an InputJumpTable must be default constructible.");
    };

    template<class ID, class Object>
    template<class Archive>
    bool InputJumpTable<ID, Object>::FillObject(ID at, Object& object, Archive& archive)
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return false;

        LoadObject(object, foundHandle, archive);

        return true;
    }

    template<class ID, class Object>
    std::vector<ID> InputJumpTable<ID, Object>::AllIDs() const
    {
        std::vector<ID> returnValue;
        for (auto& loop : handles)
            returnValue.push_back(loop.id);
        return returnValue;
    }

    template<class ID, class Object>
    bool InputJumpTable<ID, Object>::Contains(ID id) const
    {
        for (auto& loop : handles)
            if (loop.id == id)
                return true;

        return false;
    }

    template<class ID, class Object>
    size_t InputJumpTable<ID, Object>::Size() const
    {
        return handles.size();
    }

    template<class ID, class Object>
    template<class Archive>
    void InputJumpTable<ID, Object>::LoadObject(Object& object, iterator at, Archive& archive)
    {
        auto currentPosition = archive.CurrentStreamPosition();
        archive.SeekStreamFromCurrent(at->jumpPosition);
        archive(object);
        archive.SeekStreamFromCurrent(currentPosition);
    }

    template<class ID, class Object>
    template<class Archive, class Function>
    void InputJumpTable<ID, Object>::LoadObject(Function function, Object& object, iterator at, Archive& archive)
    {
        auto currentPosition = archive.CurrentStreamPosition();
        archive.SeekStreamFromCurrent(at->jumpPosition);
        function(object, archive);
        archive.SeekStreamFromCurrent(currentPosition);
    }

    template<class ID, class Object>
    typename InputJumpTable<ID, Object>::iterator InputJumpTable<ID, Object>::FindHandle(ID at)
    {
        return std::find_if(handles.begin(), handles.end(),
            [&at](const Handle& handle)
            {
                return handle.id == at;
            });
    }

    template<class ID, class Object>
    class Scribe<InputJumpTable<ID, Object>>
    {
    public:
        using ObjectT = InputJumpTable<ID, Object>;
    private:
        using Handle = typename ObjectT::Handle;
        using ArchivePosition = StreamPosition;
    public:
        void Scriven(ObjectT& object, BinaryArchive& archive)
        {
            ContainerSize size;
            archive(size);

            ArchivePosition postTablePosition = 0;
            archive(postTablePosition);

            while (size-- > 0)
                LoadJump(object, archive);

            archive.SeekStreamFromCurrent(postTablePosition);
        }
    private:
        void LoadJump(ObjectT& object, BinaryArchive& archive)
        {
            ArchivePosition jumpPosition = 0;
            ID id{};

            archive(jumpPosition);
            archive(id);

            object.handles.push_back(Handle{ id, jumpPosition });
        }
    };

    template<class ID, class Object>
    struct ScribeTraits<InputJumpTable<ID, Object>, BinaryArchive> final
    {
        using Category = CompositeScribeCategory<InputJumpTable<ID, Object>>;
    };
}