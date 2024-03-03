
#pragma once

namespace inscription
{
    template<class... FuncArgs>
    class FunctionBase
    {
    public:
        virtual ~FunctionBase() = 0;
        virtual FunctionBase* Clone() const = 0;
        virtual void Execute(FuncArgs && ... args) const = 0;
    };

    template<class... FuncArgs>
    FunctionBase<FuncArgs...>::~FunctionBase()
    {}

    template<class Ret, class Obj, class... Args>
    class Functor : public FunctionBase<Args...>
    {
    private:
        typedef Ret(Obj::*Func)(Args...);
    private:
        Func func;
        Obj &obj;
    public:
        Functor(Func func, Obj &obj);
        Functor* Clone() const override;
        void Execute(Args && ... args) const override;
    };

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>::Functor(Func func, Obj &obj) : func(func), obj(obj)
    {}

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>* Functor<Ret, Obj, Args...>::Clone() const
    {
        return new Functor(*this);
    }

    template<class Ret, class Obj, class... Args>
    void Functor<Ret, Obj, Args...>::Execute(Args && ... args) const
    {
        (obj.*func)(std::forward<Args>(args)...);
    }
    // Functor

    template<class Ret, class Obj, class... Args>
    Functor<Ret, Obj, Args...>* MakeFunctor(Ret(Obj::*func)(Args...), Obj &obj)
    {
        return new Functor<Ret, Obj, Args...>(func, obj);
    }

    template<class Ret, class... Args>
    class Function : public FunctionBase<Args...>
    {
    private:
        typedef Ret(*Func)(Args...);
    private:
        Func func;
    public:
        Function(Func func);
        Function* Clone() const override;
        void Execute(Args && ... args) const override;
    };

    template<class Ret, class... Args>
    Function<Ret, Args...>::Function(Func func) : func(func)
    {}

    template<class Ret, class... Args>
    Function<Ret, Args...>* Function<Ret, Args...>::Clone() const
    {
        return new Function(*this);
    }

    template<class Ret, class... Args>
    void Function<Ret, Args...>::Execute(Args && ... args) const
    {
        (func)(args...);
    }
    // Function

    template<class Ret, class... Args>
    Function<Ret, Args...>* MakeFunction(Ret(*func)(Args...))
    {
        return new Function<Ret, Args...>(func);
    }
}