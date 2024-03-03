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
        [[nodiscard]] std::optional<Object> ConstructObject(ID at, Archive& archive);
        template<class Function, class Archive>
        [[nodiscard]] std::optional<Object> ConstructObject(Function function, ID at, Archive& archive);
        template<class Archive>
        bool FillObject(Object& object, ID at, Archive& archive);
        template<class Function, class Archive>
        bool FillObject(Function function, Object& object, ID at, Archive& archive);

        [[nodiscard]] std::vector<ID> AllIDs() const;
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
        template<class Function, class Archive>
        void LoadFunction(Function function, Object& object, iterator at, Archive& archive);
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
    auto InputJumpTable<ID, Object>::ConstructObject(ID at, Archive& archive) -> std::optional<Object>
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return {};

        Object object{};
        LoadObject(object, foundHandle, archive);
        return object;
    }

    template<class ID, class Object>
    template<class Function, class Archive>
    [[nodiscard]] std::optional<Object> InputJumpTable<ID, Object>::ConstructObject(Function function, ID at, Archive& archive)
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return {};

        Object object{};
        LoadFunction(function, object, foundHandle, archive);
        return object;
    }

    template<class ID, class Object>
    template<class Archive>
    bool InputJumpTable<ID, Object>::FillObject(Object& object, ID at, Archive& archive)
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return false;

        LoadObject(object, foundHandle, archive);

        return true;
    }

    template<class ID, class Object>
    template<class Function, class Archive>
    bool InputJumpTable<ID, Object>::FillObject(Function function, Object& object, ID at, Archive& archive)
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return false;

        LoadFunction(function, object, foundHandle, archive);

        return true;
    }

    template<class ID, class Object>
    [[nodiscard]] std::vector<ID> InputJumpTable<ID, Object>::AllIDs() const
    {
        std::vector<ID> returnValue;
        for (auto& loop : handles)
            returnValue.push_back(loop.id);
        return returnValue;
    }

    template<class ID, class Object>
    template<class Archive>
    void InputJumpTable<ID, Object>::LoadObject(Object& object, iterator at, Archive& archive)
    {
        auto currentPosition = archive.TellStream();
        archive.SeekStream(at->jumpPosition);
        archive(object);
        archive.SeekStream(currentPosition);
    }

    template<class ID, class Object>
    template<class Function, class Archive>
    void InputJumpTable<ID, Object>::LoadFunction(Function function, Object& object, iterator at, Archive& archive)
    {
        auto currentPosition = archive.TellStream();
        archive.SeekStream(at->jumpPosition);
        object = function(archive);
        archive.SeekStream(currentPosition);
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