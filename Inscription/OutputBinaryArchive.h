#pragma once

#include "BinaryArchive.h"
#include "OutputBinaryFile.h"

#include "InvalidArchiveDirection.h"

namespace Inscription
{
    class OutputBinaryArchive : public BinaryArchive
    {
    public:
        OutputBinaryArchive(
            const FilePath& path,
            const Signature& signature,
            Version version);
        OutputBinaryArchive(
            const FilePath& path,
            const Signature& signature,
            Version version,
            const TypeRegistrationContext& typeRegistrationContext);
        OutputBinaryArchive(OutputBinaryArchive&& arg);

        OutputBinaryArchive& operator=(OutputBinaryArchive&& arg);

        template<class T>
        OutputBinaryArchive& Write(T& object);
    public:
        void SeekStream(StreamPosition position) override;
        StreamPosition TellStream() override;
    protected:
        inline void WriteImpl(bool arg) { WriteToFile(arg); }
        inline void WriteImpl(signed char arg) { WriteToFile(arg); }
        inline void WriteImpl(char arg) { WriteToFile(arg); }
        inline void WriteImpl(short arg) { WriteToFile(arg); }
        inline void WriteImpl(int arg) { WriteToFile(arg); }
        inline void WriteImpl(long arg) { WriteToFile(arg); }
        inline void WriteImpl(long long arg) { WriteToFile(arg); }
        inline void WriteImpl(unsigned char arg) { WriteToFile(arg); }
        inline void WriteImpl(unsigned short arg) { WriteToFile(arg); }
        inline void WriteImpl(unsigned int arg) { WriteToFile(arg); }
        inline void WriteImpl(unsigned long arg) { WriteToFile(arg); }
        inline void WriteImpl(unsigned long long arg) { WriteToFile(arg); }
        inline void WriteImpl(float arg) { WriteToFile(arg); }
        inline void WriteImpl(double arg) { WriteToFile(arg); }
        inline void WriteImpl(const Buffer &arg) { WriteToFile(arg); }
    private:
        OutputBinaryFile file;
    private:
        void InitialSetup();
    private:
        template<class T>
        inline void WriteToFile(T& arg)
        {
            file.WriteData(arg);
        }
    };

    template<class T>
    OutputBinaryArchive& OutputBinaryArchive::Write(T& object)
    {
        static_assert(std::is_arithmetic_v<T>, "The T given to Write was not arithmetic.");

        if (!IsLittleEndian())
            ByteSwap(object);

#ifdef INSCRIPTION_COMMON_NUMERICS
        WriteImpl(static_cast<typename NumericTraits<T>::Size>(object));
#else
        WriteImpl(object);
#endif

        return *this;
    }
}