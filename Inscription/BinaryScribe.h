#pragma once

#include "Scribe.h"

#include "PointerManager.h"
#include "TrackerMap.h"
#include "TypeRegistrationContext.h"

#include "Access.h"
#include "Endian.h"
#include "Const.h"
#include "Enum.h"
#include "Buffer.h"

#include "RegisteredTypeNotFound.h"

namespace Inscription
{
    class OutputBinaryScribe;
    class InputBinaryScribe;

    template<class T>
    class Inscripter;

    class BinaryScribe : public Scribe
    {
    public:
        typedef std::string Signature;

        typedef unsigned long long StreamPosition;
        
        typedef TypeRegistrationContext<BinaryScribe> TypeRegistrationContext;
    public:
        virtual ~BinaryScribe() = 0;
    public:
        template<class T>
        inline BinaryScribe& operator()(T&& arg);

        /*
        Call this if the pointer owns its resource
        Creates the resource
        */
        template<class T>
        inline BinaryScribe& OwningPointer(T*& arg);
        /*
        Call this if the pointer does not own its own resource
        Never creates the resource
        */
        template<class T>
        inline BinaryScribe& UnowningPointer(T*& arg);
    public:
        template<class T>
        void TrackObject(T* arg);
        template<class T>
        void ReplaceTrackedObject(T& here, T& newObj);
        bool TrackObjects(bool set = true);

        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        void StartTrackingSection();
        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        // This does NOT destroy the section entries if the clear variable is false
        void StopTrackingSection(bool clear = false);
        // A tracking section will hold all entries made. Later, you can clear the section out (removes the entries from tracking)
        // This WILL destroy the section entries, and keep the section active
        void ClearTrackingSection();

        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        void CopyTrackersTo(BinaryScribe& target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this scribe will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(BinaryScribe& target);
    public:
        bool IsOutput() const;
        bool IsInput() const;

        OutputBinaryScribe* AsOutput();
        InputBinaryScribe* AsInput();
        const OutputBinaryScribe* AsOutput() const;
        const InputBinaryScribe* AsInput() const;

        const Signature& GetSignature() const;
        Version GetVersion() const;

        void MovePositionToStart();

        virtual void SeekStream(StreamPosition position) = 0;
        virtual StreamPosition TellStream() = 0;
    public:
        template<class T>
        void RegisterType();
    protected:
        Signature signature;
        Version version;
        StreamPosition postHeaderPosition;
    protected:
        BinaryScribe(Direction direction, const Signature& signature, Version version);
        BinaryScribe(Direction direction, const Signature& signature, Version version, TypeRegistrationContext typeRegistrationContext);
        BinaryScribe(BinaryScribe&& arg);

        BinaryScribe& operator=(BinaryScribe&& arg);
    protected:
        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void Process(T& arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void Process(T& arg);
        template<class T>
        void Process(T*& arg);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void ProcessSave(T& arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void ProcessSave(T& arg);
        template<class T>
        void ProcessSave(T*& arg);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void ProcessLoad(T& arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void ProcessLoad(T& arg);
        template<class T>
        void ProcessLoad(T*& arg);

        // Owning
        template<class T>
        void ProcessSaveOwningPointerImpl(T* arg);
        template<class T>
        void ProcessLoadOwningPointerImpl(T*& arg);

        // Unowning
        template<class T>
        void ProcessSaveUnowningPointerImpl(T* arg);
        template<class T>
        void ProcessLoadUnowningPointerImpl(T*& arg);

        // Normal pointers
        template<class T>
        void ProcessSavePointerImpl(T* arg);
        template<class T>
        void ProcessLoadPointerImpl(T*& arg);

        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        void WriteNumericImplementation(T arg);
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        void WriteNumericImplementation(T arg);
        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        void ReadNumericImplementation(T& arg);
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        void ReadNumericImplementation(T& arg);
        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        T ReadNumericImplementation();
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        T ReadNumericImplementation();
    protected:
        virtual void WriteImpl(bool arg) = 0;
        virtual void WriteImpl(signed char arg) = 0;
        virtual void WriteImpl(char arg) = 0;
        virtual void WriteImpl(short arg) = 0;
        virtual void WriteImpl(int arg) = 0;
        virtual void WriteImpl(long arg) = 0;
        virtual void WriteImpl(long long arg) = 0;
        virtual void WriteImpl(unsigned char arg) = 0;
        virtual void WriteImpl(unsigned short arg) = 0;
        virtual void WriteImpl(unsigned int arg) = 0;
        virtual void WriteImpl(unsigned long arg) = 0;
        virtual void WriteImpl(unsigned long long arg) = 0;
        virtual void WriteImpl(float arg) = 0;
        virtual void WriteImpl(double arg) = 0;
        virtual void WriteImpl(const Buffer& arg) = 0;

        virtual void ReadImpl(bool& arg) = 0;
        virtual void ReadImpl(char& arg) = 0;
        virtual void ReadImpl(signed char& arg) = 0;
        virtual void ReadImpl(short& arg) = 0;
        virtual void ReadImpl(int& arg) = 0;
        virtual void ReadImpl(long& arg) = 0;
        virtual void ReadImpl(long long& arg) = 0;
        virtual void ReadImpl(unsigned char& arg) = 0;
        virtual void ReadImpl(unsigned short& arg) = 0;
        virtual void ReadImpl(unsigned int& arg) = 0;
        virtual void ReadImpl(unsigned long& arg) = 0;
        virtual void ReadImpl(unsigned long long& arg) = 0;
        virtual void ReadImpl(float& arg) = 0;
        virtual void ReadImpl(double& arg) = 0;
        virtual void ReadImpl(Buffer& arg) = 0;
    private:
        const Direction direction;
    private:
        PointerManager<BinaryScribe> pointerManager;
        TrackerMap trackers;
    private:
        BinaryScribe(const BinaryScribe& arg) = delete;
        BinaryScribe& operator=(const BinaryScribe& arg) = delete;
    private:
        template<class T>
        inline void CheckConst() const
        {
            static_assert(
                !IsConst<T>::value,
                "A const object cannot be serialized. "
                "You must remove const with const_cast, or call inscription::RemoveConst in Const.h.");
        }
    private:
        TypeRegistrationContext typeRegistrationContext;

        template<class T>
        inline void EnsureTypeRegistered() const
        {
            if (!typeRegistrationContext.HasStored<T>())
                throw RegisteredTypeNotFound(typeid(T));
        }

        template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
        template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int> = 0>
        void RegisterTypeImpl();
    };

    template<class T>
    BinaryScribe& BinaryScribe::operator()(T&& arg)
    {
        Process(arg);
        return *this;
    }

    template<class T>
    BinaryScribe& BinaryScribe::OwningPointer(T*& arg)
    {
        EnsureTypeRegistered<T>();

        if (IsOutput())
            ProcessSaveOwningPointerImpl(arg);
        else
            ProcessLoadOwningPointerImpl(arg);

        return *this;
    }

    template<class T>
    BinaryScribe& BinaryScribe::UnowningPointer(T*& arg)
    {
        EnsureTypeRegistered<T>();

        if (IsOutput())
            ProcessSaveUnowningPointerImpl(arg);
        else
            ProcessLoadUnowningPointerImpl(arg);

        return *this;
    }

    template<class T>
    void BinaryScribe::TrackObject(T* arg)
    {
        trackers.Add(arg);
    }

    template<class T>
    void BinaryScribe::ReplaceTrackedObject(T& here, T& newObj)
    {
        trackers.ReplaceObject(here, newObj);
    }

    template<class T>
    void BinaryScribe::RegisterType()
    {
        static_assert(std::is_class_v<T>, "A registered type must be a class type.");

        typeRegistrationContext.RegisterType<T>();
        RegisterTypeImpl<T>();
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void BinaryScribe::Process(T& arg)
    {
        TrackObject(&arg);
        ::Inscription::Serialize(*this, arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void BinaryScribe::Process(T& arg)
    {
        switch (direction)
        {
        case Direction::OUTPUT:
            WriteNumericImplementation(arg);
            break;
        case Direction::INPUT:
            ReadNumericImplementation(arg);
            break;
        }
    }

    template<class T>
    void BinaryScribe::Process(T*& arg)
    {
        (IsOutput()) ? ProcessSavePointerImpl(arg) : ProcessLoadPointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void BinaryScribe::ProcessSave(T& arg)
    {
        CheckConst<T>();
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void BinaryScribe::ProcessSave(T& arg)
    {
        CheckConst<T>();
        WriteNumericImplementation(arg);
    }

    template<class T>
    void BinaryScribe::ProcessSave(T*& arg)
    {
        CheckConst<T>();
        ProcessSavePointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void BinaryScribe::ProcessLoad(T& arg)
    {
        CheckConst<T>();
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void BinaryScribe::ProcessLoad(T& arg)
    {
        CheckConst<T>();
        ReadNumericImplementation(arg);
    }

    template<class T>
    void BinaryScribe::ProcessLoad(T*& arg)
    {
        CheckConst<T>();
        ProcessLoadPointerImpl(arg);
    }

    template<class T>
    void BinaryScribe::ProcessSaveOwningPointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleOwning(arg, *this, trackers);
    }

    template<class T>
    void BinaryScribe::ProcessLoadOwningPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleOwning(arg, *this, trackers);
    }

    template<class T>
    void BinaryScribe::ProcessSaveUnowningPointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void BinaryScribe::ProcessLoadUnowningPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void BinaryScribe::ProcessSavePointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void BinaryScribe::ProcessLoadPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    void BinaryScribe::WriteNumericImplementation(T arg)
    {
        static_assert(std::is_arithmetic<T>::value, "WriteNumeric was called with a non-numeric argument.");
        CheckConst<T>();

        if (!IsLittleEndian())
            ByteSwap(arg);

#ifdef INSCRIPTION_COMMON_NUMERICS
        WriteImpl(static_cast<typename NumericTraits<T>::Size>(arg));
#else
        WriteImpl(arg);
#endif
    }

    template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type>
    void BinaryScribe::WriteNumericImplementation(T arg)
    {
        CheckConst<T>();
        WriteNumericImplementation(CastEnumToUnderlying(arg));
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    void BinaryScribe::ReadNumericImplementation(T& arg)
    {
        static_assert(std::is_arithmetic<T>::value, "ReadNumeric was called with a non-numeric argument.");
        CheckConst<T>();

#ifdef INSCRIPTION_COMMON_NUMERICS
        ReadImpl(reinterpret_cast<typename NumericTraits<T>::Size&>(arg));
#else
        ReadImpl(arg);
#endif

        if (!IsLittleEndian())
            ByteSwap(arg);
    }

    template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type>
    void BinaryScribe::ReadNumericImplementation(T& arg)
    {
        CheckConst<T>();
        ReadNumericImplementation(CastEnumToUnderlyingRef(arg));
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    T BinaryScribe::ReadNumericImplementation()
    {
        static_assert(std::is_arithmetic<T>::value, "ReadNumeric was called with a non-numeric argument.");
        CheckConst<T>();

        T obj;

#ifdef INSCRIPTION_COMMON_NUMERICS
        ReadImpl(reinterpret_cast<typename NumericTraits<T>::Size&>(obj));
#else
        ReadImpl(obj);
#endif

        if (!IsLittleEndian())
            ByteSwap(obj);

        return obj;
    }

    template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type>
    T BinaryScribe::ReadNumericImplementation()
    {
        CheckConst<T>();
        return static_cast<T>(ReadNumericImplementation<std::underlying_type<T>::type>());
    }

    template<class T, std::enable_if_t<!std::is_abstract_v<T> && std::is_polymorphic_v<T>, int>>
    void BinaryScribe::RegisterTypeImpl()
    {
        pointerManager.Register<T>(Access::template InscripterT<T>::classNameResolver.NameFor(*this));
        trackers.Register<T>();
    }

    template<class T, std::enable_if_t<std::is_abstract_v<T> || !std::is_polymorphic_v<T>, int>>
    void BinaryScribe::RegisterTypeImpl()
    {
        trackers.Register<T>();
    }
}

#include "PointerDefinitions.h"