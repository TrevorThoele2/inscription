#pragma once

#include <vector>
#include <optional>
#include <algorithm>
#include <Chroma/IsBracesConstructible.h>

#include "ContainerSize.h"

#include "StreamPosition.h"
#include "CompositeScribe.h"
#include "StreamPositionScribe.h"

namespace Inscription
{
    template<class ID, class Object>
    class InputJumpTable
    {
    public:
        template<class Archive>
        [[nodiscard]] std::optional<Object> LoadObject(ID at, Archive& archive);
    private:
        struct Handle
        {
            ID id;

            StreamPosition jumpPosition;
        };

        std::vector<Handle> handles;
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(
            ::Chroma::is_braces_default_constructible_v<ID>,
            "The ID used in an InputJumpTable must be default constructible.");
        static_assert(
            ::Chroma::is_braces_default_constructible_v<Object>,
            "The Object used in an InputJumpTable must be default constructible.");
    };

    template<class ID, class Object>
    template<class Archive>
    auto InputJumpTable<ID, Object>::LoadObject(ID at, Archive& archive) -> std::optional<Object>
    {
        auto foundHandle = std::find_if(handles.begin(), handles.end(),
            [&at](const Handle& handle)
            {
                return handle.id == at;
            });

        if (foundHandle == handles.end())
            return {};

        Object object{};

        auto currentPosition = archive.TellStream();
        archive.SeekStream(foundHandle->jumpPosition);
        archive(object);
        archive.SeekStream(currentPosition);

        return object;
    }

    template<class ID, class Object, class Archive>
    class Scribe<InputJumpTable<ID, Object>, Archive> :
        public CompositeScribe<InputJumpTable<ID, Object>, Archive>
    {
    private:
        using BaseT = CompositeScribe<InputJumpTable<ID, Object>, Archive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    private:
        using Handle = typename ObjectT::Handle;
        using ArchivePosition = StreamPosition;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            ContainerSize size;
            archive(size);

            ArchivePosition postTablePosition = 0;
            archive(postTablePosition);

            while (size-- > 0)
                LoadJump(object, archive);

            archive.SeekStream(postTablePosition);
        }
    private:
        void LoadJump(ObjectT& object, ArchiveT& archive)
        {
            ArchivePosition jumpPosition = 0;
            ID id{};

            archive(jumpPosition);
            archive(id);

            object.handles.push_back(Handle{ id, jumpPosition });
        }
    };
}