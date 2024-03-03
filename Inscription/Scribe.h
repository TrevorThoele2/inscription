#pragma once

#include "Access.h"

#include "Pointer.h"
#include "Tracking.h"
#include "Path.h"
#include "Version.h"
#include "Direction.h"
#include "Numeric.h"
#include "Endian.h"
#include "Const.h"
#include "Enum.h"
#include "Buffer.h"
#include "RegisteredTypes.h"

#include "Exception.h"
#include "RegisteredTypeNotFoundException.h"

namespace Inscription
{
    class ScribeDirectionException : public Exception
    {
    public:
        ScribeDirectionException();
    };

    class ScribeExistenceException : public Exception
    {
    public:
        ScribeExistenceException();
    };

    class Scribe
    {
    public:
        typedef unsigned long long StreamPosition;
        typedef ::Inscription::Path Path;
    public:
        virtual ~Scribe() = 0;
        template<class T>
        inline Scribe& operator()(T&& arg);
        template<class T>
        inline Scribe& Save(T&& arg);
        template<class T>
        inline Scribe& Load(T&& arg);

        /*
        Call this if the pointer owns its resource
        Creates the resource
        */
        template<class T>
        inline Scribe& OwningPointer(T*& arg);
        /*
        Call this if the pointer owns its resource
        Creates the resource
        */
        template<class T>
        inline Scribe& SaveOwningPointer(T* arg);
        /*
        Call this if the pointer owns its resource
        Creates the resource
        */
        template<class T>
        inline Scribe& LoadOwningPointer(T*& arg);
        /*
        Call this if the pointer does not own its own resource
        Never creates the resource
        */
        template<class T>
        inline Scribe& UnowningPointer(T*& arg);
        /*
        Call this if the pointer does not own its own resource
        Never creates the resource
        */
        template<class T>
        inline Scribe& SaveUnowningPointer(T* arg);
        /*
        Call this if the pointer does not own its own resource
        Never creates the resource
        */
        template<class T>
        inline Scribe& LoadUnowningPointer(T*& arg);

        template<class T>
        void WriteNumeric(T arg);
        template<class T>
        void ReadNumeric(T& arg);
        template<class T>
        T ReadNumeric();

        void WriteBuffer(const Buffer& write);
        void ReadBuffer(Buffer& read);
        Buffer ReadBuffer();

        template<class T>
        bool Is() const;
        bool IsOutput() const;
        bool IsInput() const;
        virtual Version GetVersion() const = 0;

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
        void CopyTrackersTo(Scribe& target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this scribe will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(Scribe& target);

        // Will reset this back to as if it was newly created (skips the header)
        void Reset();

        template<class T>
        static void RegisterType();

        virtual void SeekStream(StreamPosition pos) = 0;
        virtual StreamPosition TellStream() = 0;
    protected:
        Scribe(Direction direction);
        Scribe(Scribe &&arg);
        Scribe& operator=(Scribe &&arg);
        inline Direction GetDirection() const { return direction; }
    private:
        PointerManager pointerManager;
        Direction direction;
        TrackerGroup trackers;

        Scribe(const Scribe& arg) = delete;
        Scribe& operator=(const Scribe& arg) = delete;

        template<class T>
        inline void CheckConst() const
        {
            static_assert(!IsConst<T>::value, "A const object cannot be serialized. You must remove const with const_cast, or call inscription::RemoveConst in Const.h.");
        }

        template<class T>
        inline void EnsureTypeRegistered() const
        {
            if (!RegisteredTypes::Has<T>())
                throw RegisteredTypeNotFoundException();
        }

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

        // Checks if going the wrong direction
        void OnExecuteOutput() const;
        // Checks if going the wrong direction
        void OnExecuteInput() const;

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

        virtual StreamPosition GetPostHeaderPosition() const = 0;

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

        friend PointerManager;
    };

    template<class T>
    Scribe& Scribe::operator()(T&& arg)
    {
        Process(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::Save(T&& arg)
    {
        OnExecuteOutput();
        ProcessSave(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::Load(T&& arg)
    {
        OnExecuteInput();
        ProcessLoad(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::OwningPointer(T*& arg)
    {
        if (!RegisteredTypes::Has<T>())
            throw RegisteredTypeNotFoundException();

        if (IsOutput())
            return SaveOwningPointer(arg);
        else
            return LoadOwningPointer(arg);
    }

    template<class T>
    Scribe& Scribe::SaveOwningPointer(T* arg)
    {
        ProcessSaveOwningPointerImpl(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::LoadOwningPointer(T*& arg)
    {
        ProcessLoadOwningPointerImpl(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::UnowningPointer(T*& arg)
    {
        if (!RegisteredTypes::Has<T>())
            throw RegisteredTypeNotFoundException();

        if (IsOutput())
            return SaveUnowningPointer(arg);
        else
            return LoadUnowningPointer(arg);
    }

    template<class T>
    Scribe& Scribe::SaveUnowningPointer(T* arg)
    {
        ProcessSaveUnowningPointerImpl(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::LoadUnowningPointer(T*& arg)
    {
        ProcessLoadUnowningPointerImpl(arg);
        return *this;
    }

    template<class T>
    void Scribe::WriteNumeric(T arg)
    {
        WriteNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ReadNumeric(T& arg)
    {
        ReadNumericImplementation(arg);
    }

    template<class T>
    T Scribe::ReadNumeric()
    {
        return ReadNumericImplementation<T>();
    }

    template<class T>
    bool Scribe::Is() const
    {
        return typeid(this) == typeid(T*);
    }

    template<class T>
    void Scribe::TrackObject(T* arg)
    {
        trackers.Add(arg);
    }

    template<class T>
    void Scribe::ReplaceTrackedObject(T& here, T& newObj)
    {
        trackers.ReplaceObject(here, newObj);
    }

    template<class T>
    void Scribe::RegisterType()
    {
        RegisteredTypes::Register<T>();
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::Process(T& arg)
    {
        TrackObject(&arg);
        ::Inscription::Serialize(*this, arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::Process(T& arg)
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
    void Scribe::Process(T*& arg)
    {
        (IsOutput()) ? ProcessSavePointerImpl(arg) : ProcessLoadPointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::ProcessSave(T& arg)
    {
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::ProcessSave(T& arg)
    {
        WriteNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ProcessSave(T*& arg)
    {
        ProcessSavePointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::ProcessLoad(T& arg)
    {
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::ProcessLoad(T& arg)
    {
        ReadNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ProcessLoad(T*& arg)
    {
        ProcessLoadPointerImpl(arg);
    }

    template<class T>
    void Scribe::ProcessSaveOwningPointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleOwning(arg, *this, trackers);
    }

    template<class T>
    void Scribe::ProcessLoadOwningPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleOwning(arg, *this, trackers);
    }

    template<class T>
    void Scribe::ProcessSaveUnowningPointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void Scribe::ProcessLoadUnowningPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void Scribe::ProcessSavePointerImpl(T* arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T>
    void Scribe::ProcessLoadPointerImpl(T*& arg)
    {
        CheckConst<T>();
        EnsureTypeRegistered<T>();
        pointerManager.HandleUnowning(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    void Scribe::WriteNumericImplementation(T arg)
    {
        static_assert(std::is_arithmetic<T>::value, "WriteNumeric was called with a non-numeric argument.");
        OnExecuteOutput();

        if (!IsLittleEndian())
            ByteSwap(arg);

#ifdef INSCRIPTION_COMMON_NUMERICS
        WriteImpl(static_cast<typename NumericTraits<T>::Size>(arg));
#else
        WriteImpl(arg);
#endif
    }

    template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type>
    void Scribe::WriteNumericImplementation(T arg)
    {
        WriteNumericImplementation(CastEnumToUnderlying(arg));
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    void Scribe::ReadNumericImplementation(T& arg)
    {
        static_assert(std::is_arithmetic<T>::value, "ReadNumeric was called with a non-numeric argument.");
        OnExecuteInput();

#ifdef INSCRIPTION_COMMON_NUMERICS
        ReadImpl(reinterpret_cast<typename NumericTraits<T>::Size&>(arg));
#else
        ReadImpl(arg);
#endif

        if (!IsLittleEndian())
            ByteSwap(arg);
    }

    template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type>
    void Scribe::ReadNumericImplementation(T& arg)
    {
        ReadNumericImplementation(CastEnumToUnderlyingRef(arg));
    }

    template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type>
    T Scribe::ReadNumericImplementation()
    {
        static_assert(std::is_arithmetic<T>::value, "ReadNumeric was called with a non-numeric argument.");
        OnExecuteInput();

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
    T Scribe::ReadNumericImplementation()
    {
        return static_cast<T>(ReadNumericImplementation<std::underlying_type<T>::type>());
    }
}

#include "PointerDefinitions.h"