#pragma once

/*
Defines for your convenience
*/

//////////
// SYMBOLS
#ifndef INSCRIPTION_SCRIBE_SYMBOL
    #define INSCRIPTION_SCRIBE_SYMBOL scribe
#endif

#ifndef INSCRIPTION_TABLE_SYMBOL
    #define INSCRIPTION_TABLE_SYMBOL table
#endif

#ifndef INSCRIPTION_OBJECT_SYMBOL
    #define INSCRIPTION_OBJECT_SYMBOL obj
#endif

#ifndef INSCRIPTION_CLASS_NAME_RESOLVER_SYMBOL
    #define INSCRIPTION_CLASS_NAME_RESOLVER_SYMBOL localContainer
#endif

/////////
// ACCESS
#define INSCRIPTION_ACCESS friend ::Inscription::Access;    \
template<class T>                                           \
friend class ::Inscription::Inscripter;

/////////////////////
// INSCRIPTER DEFINES
// Place the class name container inside of the inscripter
#define INSCRIPTION_INSCRIPTER_DECLARE(type)        \
template<>                                          \
class Inscripter<type> : public InscripterBase<type>

#define INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(type)  \
typedef InscripterBase<type> BaseT;                 \
typedef typename BaseT::TableT TableT;              \
typedef typename BaseT::ManagedT ManagedT;

#define INSCRIPTION_INSCRIPTER_CONSTRUCTOR_DECLARE(type)    \
type(const ::Inscription::Table<type>& table)

#define INSCRIPTION_INSCRIPTER_CONSTRUCTOR_DEFINE(type)    \
type::type(const ::Inscription::Table<type>& table)

#define INSCRIPTION_INSCRIPTER_DECLARE_TABLE            \
static TableT CreateTable(::Inscription::Scribe& scribe)

#define INSCRIPTION_INSCRIPTER_DEFINE_TABLE(type)                                       \
typename Inscripter<type>::TableT Inscripter<type>::CreateTable(::Inscription::Scribe& scribe)

#define INSCRIPTION_INSCRIPTER_TABLE                    \
static TableT CreateTable(::Inscription::Scribe& scribe)

#define INSCRIPTION_INSCRIPTER_CREATE_TABLE \
TableT INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_INSCRIPTER_RETURN_TABLE \
return INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION                                                                                           \
static void SerializeTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(type)                                                                                                  \
void Inscripter<type>::SerializeTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_SERIALIZE_FUNCTION                                                                                                   \
static void SerializeTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION                                                                 \
InscripterBase<ManagedT>::SerializeTable(INSCRIPTION_TABLE_SYMBOL, INSCRIPTION_SCRIBE_SYMBOL, INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DECLARE_CONSTRUCT_FUNCTION                                       \
static void ConstructTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_CONSTRUCT_FUNCTION(type)                                              \
void Inscripter<type>::ConstructTable(TableT &INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DECLARE_CONSTRUCT_OBJECT_FUNCTION    \
static void ConstructObjectInstance(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, ManagedT*& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_CONSTRUCT_OBJECT_FUNCTION(type)                                                   \
void Inscripter<type>::ConstructObjectInstance(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, ManagedT*& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFAULT_SERIALIZE_FUNCTION  \
using InscripterBase<ManagedT>::SerializeTable

#define INSCRIPTION_INSCRIPTER_DEFAULT_CONSTRUCT_FUNCTION  \
using InscripterBase<ManagedT>::ConstructTable

#define INSCRIPTION_INSCRIPTER_DECLARE_POST_CONSTRUCT   \
static void PostConstruct(ManagedT& INSCRIPTION_OBJECT_SYMBOL);

#define INSCRIPTION_INSCRIPTER_DEFINE_POST_CONSTRUCT(type)  \
void Inscripter<type>::PostConstruct(ManagedT& INSCRIPTION_OBJECT_SYMBOL)

/////////////////////
// CLASS NAME DEFINES
#define INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER                         \
static ::Inscription::ClassNameResolver classNameResolver;

#define INSCRIPTION_DEFINE_CLASS_NAME_RESOLVER(type)                            \
::Inscription::ClassNameResolver Inscripter<type>::classNameResolver

#define INSCRIPTION_CLASS_NAME_RESOLVER                                 \
static ::Inscription::ClassNameResolver classNameResolver

#define INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(type, defaultName)                \
::Inscription::ClassNameResolver Inscripter<type>::classNameResolver([](Scribe& scribe) \
{                                                                                       \
    return defaultName;                                                                 \
});

////////////////////////
// SERIALIZATION DEFINES
#define INSCRIPTION_SERIALIZATION \
static void SerializeTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL, Managed& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_SERIALIZE_TABLE \
INSCRIPTION_TABLE_SYMBOL.Serialize(INSCRIPTION_SCRIBE_SYMBOL, INSCRIPTION_OBJECT_SYMBOL);

#define INSCRIPTION_CONSTRUCTION \
static void ConstructTable(TableT& INSCRIPTION_TABLE_SYMBOL, ::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)

#define INSCRIPTION_CONSTRUCT_TABLE \
INSCRIPTION_TABLE_SYMBOL.Construct(INSCRIPTION_SCRIBE_SYMBOL);

#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION void INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION void Serialize(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#endif

#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION_DECLARE void INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL);
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION_DECLARE void Serialize(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL);
#endif

#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(type) void type::INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(type) void type::Serialize(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#endif

////////////////
// TABLE DEFINES
#define INSCRIPTION_TABLE_BEGIN                                             \
static TableT CreateTable(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL) \
{                                                                           \
    TableT INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_TABLE_END           \
    return INSCRIPTION_TABLE_SYMBOL;    \
}

#define INSCRIPTION_NULL_TABLE  \
INSCRIPTION_TABLE_BEGIN         \
INSCRIPTION_TABLE_END

// Just serializes the variable. Intended to be used inside custom inscripter functions
#define INSCRIPTION_SERIALIZE_MEM(name) INSCRIPTION_SCRIBE_SYMBOL(INSCRIPTION_OBJECT_SYMBOL.name);
// Just saves the variable. Intended to be used inside custom inscripter functions
#define INSCRIPTION_SAVE_MEM(name) INSCRIPTION_SCRIBE_SYMBOL.Save(INSCRIPTION_OBJECT_SYMBOL.name);
// Just loads the variable. Intended to be used inside custom inscripter functions
#define INSCRIPTION_LOAD_MEM(name) INSCRIPTION_SCRIBE_SYMBOL.Load(INSCRIPTION_OBJECT_SYMBOL.name);

// Adds the external into the table. This must be linked when outputting from the serialize function.
// Explictly define the type
#define INSCRIPTION_TABLE_ADD_EXTERNAL_EX(type, name) INSCRIPTION_TABLE_SYMBOL.AddLinkable<type>(name);

// Adds the class variable into the table
#define INSCRIPTION_TABLE_ADD(var) \
INSCRIPTION_TABLE_SYMBOL.Add<decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var),& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
// Adds the class variable into the table. Will call Scribe::AssumeExist family of functions
#define INSCRIPTION_TABLE_ADD_OWNING_POINTER(var) \
INSCRIPTION_TABLE_SYMBOL.AddOwningPointer<decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var),& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
// Adds the class variable into the table. Will call Scribe:SimplePointer family of functions
#define INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(var) \
INSCRIPTION_TABLE_SYMBOL.AddUnowningPointer<decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var),& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
// Adds the reference into the table. This must be linked when outputting from the serialize function.
#define INSCRIPTION_TABLE_ADD_REF(var) \
INSCRIPTION_TABLE_SYMBOL.AddLinkable<decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var)>(#var);
// Adds the external into the table. This must be linked when outputting from the serialize function.
#define INSCRIPTION_TABLE_ADD_EXTERNAL(var) \
INSCRIPTION_TABLE_ADD_EXTERNAL_EX(decltype(var), #var);
// Adds an imposter into the table. This must be linked from both the serialize and construction functions
#define INSCRIPTION_TABLE_ADD_IMPOSTER(var, fileT) \
INSCRIPTION_TABLE_SYMBOL.AddImposter<decltype(var), fileT>(#var);
// Adds a base into the table
#define INSCRIPTION_TABLE_ADD_BASE(baseT) \
INSCRIPTION_TABLE_SYMBOL.AddBase<baseT>();

// Links the variable when you serialize
#define INSCRIPTION_TABLE_LINK(var) \
INSCRIPTION_TABLE_SYMBOL.Link(#var, INSCRIPTION_OBJECT_SYMBOL.var);
// Links external when you serialize
#define INSCRIPTION_TABLE_LINK_EXTERNAL(varName, var) \
INSCRIPTION_TABLE_SYMBOL.Link(varName, std::move(var));

// Retrieves the variable out of the table
#define INSCRIPTION_TABLE_GET(var) \
INSCRIPTION_TABLE_SYMBOL.Get<decltype(var)>(#var)
// Retrieves the variable out of the table.
// Explicitly define the type
#define INSCRIPTION_TABLE_GET_EX(varType, varName) \
INSCRIPTION_TABLE_SYMBOL.Get<varType>(varName)

// Moves the variable out of the table
// Will work with copy constructors
#define INSCRIPTION_TABLE_GET_MEM(var) var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)))
// Forces a copy out of the table instead of moving
#define INSCRIPTION_TABLE_GET_MEM_COPY(var) var(INSCRIPTION_TABLE_GET(var))
// Extracts the constant argument from the table
#define INSCRIPTION_TABLE_GET_MEM_CONST(var) var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_const<decltype(var)>::type>(#var)))
// Defines arguments before the get call
#define INSCRIPTION_TABLE_GET_MEM_PRE(var, ...) var(__VA_ARGS__, std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)))
// Defines arguments after the get call
#define INSCRIPTION_TABLE_GET_MEM_POST(var, ...) var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)), __VA_ARGS__)
// Sets up the base class from the table
#define INSCRIPTION_TABLE_GET_BASE(baseT) baseT(INSCRIPTION_TABLE_SYMBOL.Convert<baseT>())

#define INSCRIPTION_TABLE_ASSIGN(var) var = INSCRIPTION_TABLE_SYMBOL.Get<decltype(var)>(#var)

#define INSCRIPTION_TABLE_CONSTRUCTOR_DECLARE(type) type(const ::Inscription::Table<type>& INSCRIPTION_TABLE_SYMBOL);
#define INSCRIPTION_TABLE_CONSTRUCTOR_SIMPLE_DECLARE(type, endingAccess)    \
public:                                                                     \
INSCRIPTION_TABLE_CONSTRUCTOR_DECLARE(type)                                 \
INSCRIPTION_SIMPLE_INSCRIPTER_ACCESS                                        \
endingAccess:

#define INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(type) \
type::type(const ::Inscription::Table<type>& INSCRIPTION_TABLE_SYMBOL)

#define INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_EX(type, realT)   \
type::realT(const ::Inscription::Table<type>& INSCRIPTION_TABLE_SYMBOL)

#define INSCRIPTION_TABLE_CONSTRUCTOR(type)    \
type(const ::Inscription::Table<type>& INSCRIPTION_TABLE_SYMBOL)