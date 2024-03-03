
#include <exception>
#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "Const.h"

namespace inscription
{
    BinaryScribeMarkerException::BinaryScribeMarkerException() : Exception("The marker in the file was not identical to the marker given. The file is either corrupt or not the right file type.")
    {}

    BinaryScribe::BinaryScribe(const Path &path, const Marker &marker, Version version) : Scribe(Direction::OUTPUT), marker(marker), version(version), postHeaderPosition(0)
    {
        file.out = new BinaryOutputFile(path);

        ContainerSize size(marker.size());
        (*this)(size);
        for (auto &loop : marker)
            Save(RemoveConst(loop));

        (*this)(version);
        postHeaderPosition = TellStream();
    }

    BinaryScribe::BinaryScribe(const Path &path, const Marker &marker) : Scribe(Direction::INPUT), postHeaderPosition(0)
    {
        file.in = new BinaryInputFile(path);

        // load the name
        Marker loadMarker;
        ContainerSize size;
        Load(size);

        loadMarker.resize(size.Get());
        ContainerSize::ValueT address = 0;
        while (address < size.Get())
        {
            Load(loadMarker[address]);
            ++address;
        }

        // If the marker in the file is not identical to the marker given, throw an exception
        if (loadMarker != marker)
            throw BinaryScribeMarkerException();

        Load(version);
        this->marker = loadMarker;
        postHeaderPosition = TellStream();
    }

    BinaryScribe::BinaryScribe(BinaryScribe &&arg) : Scribe(std::move(arg)), file(std::move(arg.file)), marker(std::move(arg.marker)), version(std::move(arg.version)), postHeaderPosition(std::move(arg.postHeaderPosition))
    {}

    BinaryScribe& BinaryScribe::operator=(BinaryScribe &&arg)
    {
        Scribe::operator=(std::move(arg));
        file = std::move(arg.file);
        marker = std::move(arg.marker);
        version = std::move(arg.version);
        postHeaderPosition = std::move(arg.postHeaderPosition);
        return *this;
    }

    BinaryScribe::~BinaryScribe()
    {
        (IsOutput()) ? delete file.out : delete file.in;
    }

    const BinaryScribe::Marker& BinaryScribe::GetMarker() const
    {
        return marker;
    }

    Version BinaryScribe::GetVersion() const
    {
        return version;
    }

    void BinaryScribe::SeekStream(StreamPos pos)
    {
        switch (GetDirection())
        {
        case Direction::OUTPUT:
            file.out->SeekStream(pos);
            break;
        case Direction::INPUT:
            file.in->SeekStream(pos);
            break;
        }
    }

    BinaryScribe::StreamPos BinaryScribe::TellStream()
    {
        // Needs to end on the else, or suffer the wrath of COMPILER WARNINGS
        if (GetDirection() == Direction::OUTPUT)
            return file.out->TellStream();
        else
            return file.in->TellStream();
    }

    BinaryScribe::StreamPos BinaryScribe::GetPostHeaderPosition() const
    {
        return postHeaderPosition;
    }
}