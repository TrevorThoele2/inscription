#include "BufferScribe.h"

#include "OutputBinaryFormat.h"
#include "InputBinaryFormat.h"

namespace Inscription
{
    void Scribe<Buffer>::Scriven(ObjectT& object, Format::Binary& format)
    {
        if (format.IsOutput())
            format.AsOutput()->Write(object);
        else
            format.AsInput()->Read(object);
    }
}