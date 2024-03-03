#pragma once

#include "Access.h"

#include "Polymorphic.h"
#include "Tracking.h"
#include "TrackingTraits.h"
#include "Path.h"
#include "Version.h"
#include "Direction.h"
#include "Numeric.h"
#include "Endian.h"
#include "Const.h"
#include "Enum.h"
#include "Buffer.h"

#include "Exception.h"

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
        typedef unsigned long long StreamPos;
        typedef ::Inscription::Path Path;
    public:
        virtual ~Scribe() = 0;
        template<class... Args>
        inline Scribe& operator()(Args && ... args);
        template<class... Args>
        inline Scribe& Save(Args && ... args);
        template<class... Args>
        inline Scribe& Load(Args && ... args);
        /*
        Call this if the pointer already exists prior to this call
        This will skip anything pertaining to creating this object
        This will use tracking to find the object, so it must be tracked beforehand
        */
        template<class T>
        inline Scribe& AssumeExists(T *&arg);
        /*
        Call this if the pointer already exists prior to this call
        This will skip anything pertaining to creating this object
        This will use tracking to find the object, so it must be tracked beforehand
        */
        template<class T>
        inline Scribe& AssumeExistsSave(T *arg);
        /*
        Call this if the pointer already exists prior to this call
        This will skip anything pertaining to creating this object
        This will use tracking to find the object, so it must be tracked beforehand
        */
        template<class T>
        inline Scribe& AssumeExistsLoad(T *&arg);

        // Will never create the pointer, but can handle nullptrs
        template<class T>
        inline Scribe& SimplePointer(T *&arg);
        // Will never create the pointer, but can handle nullptrs
        template<class T>
        inline Scribe& SimplePointerSave(T *arg);
        // Will never create the pointer, but can handle nullptrs
        template<class T>
        inline Scribe& SimplePointerLoad(T *&arg);

        template<class T>
        void WriteNumeric(T arg);
        template<class T>
        void ReadNumeric(T &arg);
        template<class T>
        T ReadNumeric();

        void WriteBuffer(const Buffer &write);
        void ReadBuffer(Buffer &read);
        Buffer ReadBuffer();

        template<class T>
        bool Is() const;
        bool IsOutput() const;
        bool IsInput() const;
        virtual Version GetVersion() const = 0;

        template<class T>
        void TrackObject(T *arg);
        template<class T>
        void ReplaceTrackedObject(T &here, T &newObj);
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
        void CopyTrackersTo(Scribe &target) const;
        // Will remove all of the target's tracking history
        // For this reason, this should probably be called at the beginning of the target's lifetime
        // Be sure that this scribe will no longer need to handle pointers through tracking when calling this
        void MoveTrackersTo(Scribe &target);

        // Will reset this back to as if it was newly created (skips the header)
        void Reset();

        template<class T>
        static void RegisterPolymorphic();

        virtual void SeekStream(StreamPos pos) = 0;
        virtual StreamPos TellStream() = 0;
    protected:
        Scribe(Direction direction);
        Scribe(Scribe &&arg);
        Scribe& operator=(Scribe &&arg);
        inline Direction GetDirection() const { return direction; }
    private:
        PolymorphicManager polyManager;
        Direction direction;
        TrackerGroup trackers;

        Scribe(const Scribe &arg) = delete;
        Scribe& operator=(const Scribe &arg) = delete;

        template<class T>
        void UnpackArguments(T &&arg);
        template<class T, class... Args>
        void UnpackArguments(T &&first, Args && ... args);
        template<class T>
        void UnpackArgumentsSave(T &&arg);
        template<class T, class... Args>
        void UnpackArgumentsSave(T &&first, Args && ... args);
        template<class T>
        void UnpackArgumentsLoad(T &&arg);
        template<class T, class... Args>
        void UnpackArgumentsLoad(T &&first, Args && ... args);

        template<class T>
        inline void CheckConst() const
        {
            static_assert(!IsConst<T>::value, "A const object cannot be serialized. You must remove const with const_cast, or call inscription::RemoveConst in Const.h.");
        }

        template<class T>
        inline void CheckPointer() const
        {
            static_assert(TrackPointer<T>::value != TRACK_NEVER, "This type cannot be serialized through a pointer.");
        }

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void Process(T &arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void Process(T &arg);
        template<class T>
        void Process(T *&arg);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void ProcessSave(T &arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void ProcessSave(T &arg);
        template<class T>
        void ProcessSave(T *&arg);

        template<class T, typename std::enable_if<std::is_class<T>::value, int>::type = 0>
        void ProcessLoad(T &arg);
        template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type = 0>
        void ProcessLoad(T &arg);
        template<class T>
        void ProcessLoad(T *&arg);

        template<class T>
        void SaveTracked(T *arg);
        template<class T>
        void LoadTracked(T *&arg);
        template<class T>
        void SimpleSaveTracked(T *&arg);
        template<class T>
        void SimpleLoadTracked(T *&arg);

        // Normal pointers
        template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessSavePointerImpl(T *arg);
        template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessSavePointerImpl(T *arg);
        template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessLoadPointerImpl(T *&arg);
        template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessLoadPointerImpl(T *&arg);

        // Simple
        template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessSaveSimplePointerImpl(T *arg);
        template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessSaveSimplePointerImpl(T *arg);
        template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessLoadSimplePointerImpl(T *&arg);
        template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type = 0>
        void ProcessLoadSimplePointerImpl(T *&arg);

        // Checks if going the wrong direction
        void OnExecuteOutput() const;
        // Checks if going the wrong direction
        void OnExecuteInput() const;

        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        void WriteNumericImplementation(T arg);
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        void WriteNumericImplementation(T arg);
        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        void ReadNumericImplementation(T &arg);
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        void ReadNumericImplementation(T &arg);
        template<class T, typename std::enable_if<!std::is_enum<T>::value, int>::type = 0>
        T ReadNumericImplementation();
        template<class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        T ReadNumericImplementation();

        virtual StreamPos GetPostHeaderPosition() const = 0;

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
        virtual void WriteImpl(const Buffer &arg) = 0;

        virtual void ReadImpl(bool &arg) = 0;
        virtual void ReadImpl(char &arg) = 0;
        virtual void ReadImpl(signed char &arg) = 0;
        virtual void ReadImpl(short &arg) = 0;
        virtual void ReadImpl(int &arg) = 0;
        virtual void ReadImpl(long &arg) = 0;
        virtual void ReadImpl(long long &arg) = 0;
        virtual void ReadImpl(unsigned char &arg) = 0;
        virtual void ReadImpl(unsigned short &arg) = 0;
        virtual void ReadImpl(unsigned int &arg) = 0;
        virtual void ReadImpl(unsigned long &arg) = 0;
        virtual void ReadImpl(unsigned long long &arg) = 0;
        virtual void ReadImpl(float &arg) = 0;
        virtual void ReadImpl(double &arg) = 0;
        virtual void ReadImpl(Buffer &arg) = 0;

        friend PolymorphicManager;
    };

    template<class... Args>
    Scribe& Scribe::operator()(Args && ... args)
    {
        UnpackArguments(std::forward<Args>(args)...);
        return *this;
    }

    template<class... Args>
    Scribe& Scribe::Save(Args && ... args)
    {
        OnExecuteOutput();
        UnpackArgumentsSave(std::forward<Args>(args)...);
        return *this;
    }

    template<class... Args>
    Scribe& Scribe::Load(Args && ... args)
    {
        OnExecuteInput();
        UnpackArgumentsLoad(std::forward<Args>(args)...);
        return *this;
    }

    template<class T>
    Scribe& Scribe::AssumeExists(T *&arg)
    {
        static_assert(TrackPointer<T>::value != TRACK_NEVER, "This type cannot be serialized through a pointer.");
        if (IsOutput())
            return AssumeExistsSave(arg);
        else
            return AssumeExistsLoad(arg);
    }

    template<class T>
    Scribe& Scribe::AssumeExistsSave(T *arg)
    {
        if (!arg)
            throw ScribeExistenceException();

        ProcessSaveSimplePointerImpl(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::AssumeExistsLoad(T *&arg)
    {
        ProcessLoadSimplePointerImpl(arg);
        if(!arg)
            throw ScribeExistenceException();

        return *this;
    }

    template<class T>
    Scribe& Scribe::SimplePointer(T *&arg)
    {
        static_assert(TrackPointer<T>::value != TRACK_NEVER, "This type cannot be serialized through a pointer.");
        if (IsOutput())
            return SimplePointerSave(arg);
        else
            return SimplePointerLoad(arg);
    }

    template<class T>
    Scribe& Scribe::SimplePointerSave(T *arg)
    {
        ProcessSaveSimplePointerImpl(arg);
        return *this;
    }

    template<class T>
    Scribe& Scribe::SimplePointerLoad(T *&arg)
    {
        ProcessLoadSimplePointerImpl(arg);
        return *this;
    }

    template<class T>
    void Scribe::WriteNumeric(T arg)
    {
        WriteNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ReadNumeric(T &arg)
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
    void Scribe::TrackObject(T *arg)
    {
        trackers.Add(arg);
    }

    template<class T>
    void Scribe::ReplaceTrackedObject(T &here, T &newObj)
    {
        trackers.ReplaceObject(here, newObj);
    }

    template<class T>
    void Scribe::RegisterPolymorphic()
    {
        PolymorphicManager::Register<T>();
    }

    template<class T>
    void Scribe::UnpackArguments(T &&arg)
    {
        CheckConst<T>();
        Process(arg);
    }

    template<class T, class... Args>
    void Scribe::UnpackArguments(T &&first, Args && ... args)
    {
        UnpackArguments(std::forward<T>(first));
        UnpackArguments(std::forward<Args>(args)...);
    }

    template<class T>
    void Scribe::UnpackArgumentsSave(T &&arg)
    {
        CheckConst<T>();
        ProcessSave(arg);
    }

    template<class T, class... Args>
    void Scribe::UnpackArgumentsSave(T &&first, Args && ... args)
    {
        UnpackArgumentsSave(std::forward<T>(first));
        UnpackArgumentsSave(std::forward<Args>(args)...);
    }

    template<class T>
    void Scribe::UnpackArgumentsLoad(T &&arg)
    {
        CheckConst<T>();
        ProcessLoad(arg);
    }

    template<class T, class... Args>
    void Scribe::UnpackArgumentsLoad(T &&first, Args && ... args)
    {
        UnpackArgumentsLoad(std::forward<T>(first));
        UnpackArgumentsLoad(std::forward<Args>(args)...);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::Process(T &arg)
    {
        TrackObject(&arg);
        Serialize(*this, arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::Process(T &arg)
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
    void Scribe::Process(T *&arg)
    {
        (IsOutput()) ? ProcessSavePointerImpl(arg) : ProcessLoadPointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::ProcessSave(T &arg)
    {
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::ProcessSave(T &arg)
    {
        WriteNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ProcessSave(T *&arg)
    {
        ProcessSavePointerImpl(arg);
    }

    template<class T, typename std::enable_if<std::is_class<T>::value, int>::type>
    void Scribe::ProcessLoad(T &arg)
    {
        Process(arg);
    }

    template<class T, typename std::enable_if<!std::is_class<T>::value, int>::type>
    void Scribe::ProcessLoad(T &arg)
    {
        ReadNumericImplementation(arg);
    }

    template<class T>
    void Scribe::ProcessLoad(T *&arg)
    {
        ProcessLoadPointerImpl(arg);
    }

    template<class T>
    void Scribe::SaveTracked(T *arg)
    {
        // Save ID always, and selectively save the object itself
        if (!arg)
        {
            WriteNumericImplementation(TrackerID(trackers.nullID));
            return;
        }

        auto &tracked = trackers.Add(arg);
        auto justAdded = tracked.first;
        auto entry = tracked.second;
        // Entry can be null if the tracker is inactive
        if (entry)
        {
            WriteNumericImplementation(entry->GetID());
            // Save real object if just added and not nullptr
            if (justAdded)
                Process(*arg);
        }
    }

    template<class T>
    void Scribe::LoadTracked(T *&arg)
    {
        // Load ID always and selectively load the object after
        TrackerID id = trackers.nullID;
        ReadNumericImplementation(id);
        if (id == trackers.nullID)
        {
            arg = nullptr;
            return;
        }

        auto found = trackers.Find<T>(id);
        // Load object
        if (found)
            // Set object if it's here
            arg = found;
        else
        {
            // Create new object if it isn't in here previously
            Construct(*this, arg);
            // Place new object into the trackers
            trackers.Add(arg);
        }
    }

    template<class T>
    void Scribe::SimpleSaveTracked(T *&arg)
    {
        static_assert(TrackPointer<T>::value != TRACK_NEVER, "This type cannot be serialized through a pointer.");
        OnExecuteOutput();

        trackers.AttemptEnableSelectiveTracking<T>();

        auto &tracked = trackers.Add(arg);
        auto justAdded = tracked.first;
        auto entry = tracked.second;
        // Entry can be null if the tracker is inactive
        if (entry)
            WriteNumericImplementation(entry->GetID());
        else
            WriteNumericImplementation(trackers.nullID);
    }

    template<class T>
    void Scribe::SimpleLoadTracked(T *&arg)
    {
        static_assert(TrackPointer<T>::value != TRACK_NEVER, "This type cannot be serialized through a pointer.");
        OnExecuteInput();

        trackers.AttemptEnableSelectiveTracking<T>();

        TrackerID id = trackers.nullID;
        ReadNumericImplementation(id);
        arg = trackers.Find<T>(id);
    }

    template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessSavePointerImpl(T *arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        polyManager.HandleObject(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessSavePointerImpl(T *arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        SaveTracked(arg);
    }

    template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessLoadPointerImpl(T *&arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        polyManager.HandleObject(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessLoadPointerImpl(T *&arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        LoadTracked(arg);
    }

    template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessSaveSimplePointerImpl(T *arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        polyManager.HandleObjectNoConstruct(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessSaveSimplePointerImpl(T *arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        SimpleSaveTracked(arg);
    }

    template<class T, typename std::enable_if<std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessLoadSimplePointerImpl(T *&arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        polyManager.HandleObjectNoConstruct(arg, *this, trackers);
    }

    template<class T, typename std::enable_if<!std::is_polymorphic<T>::value, int>::type>
    void Scribe::ProcessLoadSimplePointerImpl(T *&arg)
    {
        CheckConst<T>();
        CheckPointer<T>();
        trackers.AttemptEnableSelectiveTracking<T>();
        SimpleLoadTracked(arg);
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
    void Scribe::ReadNumericImplementation(T &arg)
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
    void Scribe::ReadNumericImplementation(T &arg)
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

#include "PolymorphicDefinitions.h"