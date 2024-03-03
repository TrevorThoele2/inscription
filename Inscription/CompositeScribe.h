#pragma once

#include "ObjectScribe.h"

namespace Inscription
{
    template<class Object, class Archive>
    class CompositeScribe;

    template<class Object>
    class CompositeScribe<Object, BinaryArchive> : public ObjectScribe<Object, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<Object, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        using BaseT::ScrivenImplementation;
    };

    template<class Object>
    class CompositeScribe<Object, JsonArchive> : public ObjectScribe<Object, JsonArchive>
    {
    private:
        using BaseT = ObjectScribe<Object, JsonArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using BaseT::Scriven;
    protected:
        CompositeScribe() = default;
        CompositeScribe(const CompositeScribe& arg) = default;

        void ScrivenImplementation(const std::string& name, ObjectT& object, ArchiveT& archive) override final;
        virtual void ScrivenImplementation(ObjectT& object, ArchiveT& archive) = 0;
    };

    template<class Object>
    void CompositeScribe<Object, JsonArchive>::ScrivenImplementation(
        const std::string& name, ObjectT& object, ArchiveT& archive)
    {
        if (archive.IsOutput())
        {
            auto output = archive.AsOutput();
            output->StartObject(name);
            ScrivenImplementation(object, archive);
            output->EndObject();
        }
        else
        {
            auto input = archive.AsInput();
            input->StartObject(name);
            ScrivenImplementation(object, archive);
            input->EndObject();
        }
    }
}