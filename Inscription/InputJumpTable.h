#pragma once

#include <vector>
#include <algorithm>
#include <Chroma/IsBracesConstructible.h>

#include "ContainerSize.h"

#include "FilePosition.h"
#include "InputBinaryFormat.h"

namespace Inscription
{
    template<class ID, class Object>
    class InputJumpTable
    {
    public:
        void Load(File::InputBinary& file, Format::Binary& format);

        template<class Format>
        bool FillObject(ID at, Object& object, Format& format);

        [[nodiscard]] std::vector<ID> AllIDs() const;
        [[nodiscard]] bool Contains(ID id) const;
        [[nodiscard]] size_t Size() const;
    private:
        struct Handle
        {
            ID id;
            File::Position jumpPosition;
        };

        using Handles = std::vector<Handle>;
        Handles handles;

        using iterator = typename Handles::iterator;
    private:
        File::InputBinary* file = nullptr;

        template<class Format>
        void LoadObject(Object& object, iterator at, Format& format);
        template<class Format, class Function>
        void LoadObject(Function function, Object& object, iterator at, Format& format);
        iterator FindHandle(ID at);

        void LoadJump(Format::Binary& format);
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(
            ::Chroma::is_braces_default_constructible_v<ID>,
            "The ID used in an InputJumpTable must be default constructible.");
    };

    template<class ID, class Object>
    void InputJumpTable<ID, Object>::Load(File::InputBinary& file, Format::Binary& format)
    {
        this->file = &file;

        ContainerSize size;
        format(size);

        File::Position postTablePosition = 0;
        format(postTablePosition);

        while (size-- > 0)
            LoadJump(format);

        file.Seek(postTablePosition);
    }

    template<class ID, class Object>
    template<class Format>
    bool InputJumpTable<ID, Object>::FillObject(ID at, Object& object, Format& format)
    {
        auto foundHandle = FindHandle(at);
        if (foundHandle == handles.end())
            return false;

        LoadObject(object, foundHandle, format);

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
    template<class Format>
    void InputJumpTable<ID, Object>::LoadObject(Object& object, iterator at, Format& format)
    {
        const auto currentPosition = file->Position();
        file->Seek(at->jumpPosition);
        format(object);
        file->Seek(currentPosition);
    }

    template<class ID, class Object>
    template<class Format, class Function>
    void InputJumpTable<ID, Object>::LoadObject(Function function, Object& object, iterator at, Format& format)
    {
        const auto currentPosition = file->Position();
        file->Seek(at->jumpPosition);
        function(object, format);
        file->Seek(currentPosition);
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
    void InputJumpTable<ID, Object>::LoadJump(Format::Binary& format)
    {
        File::Position jumpPosition = 0;
        ID id{};

        format(jumpPosition);
        format(id);

        handles.push_back(Handle{ id, jumpPosition });
    }
}