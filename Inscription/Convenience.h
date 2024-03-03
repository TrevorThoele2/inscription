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

#define INSCRIPTION_INSCRIPTER_BASE_TYPEDEFS(type)                      \
typedef InscripterBase<type> BaseT;                                     \
template<class ScribeT>                                                 \
using Table = typename InscripterBase<type>::template Table<ScribeT>;   \
typedef typename BaseT::ManagedT ManagedT;

#define INSCRIPTION_INSCRIPTER_DECLARE_TABLE(scribeType)    \
static Table<scribeType> CreateTable(scribeType& scribe)

#define INSCRIPTION_INSCRIPTER_DEFINE_TABLE(type, scribeType)                                   \
typename Inscripter<type>::Table<scribeType> Inscripter<type>::CreateTable(scribeType& scribe)

#define INSCRIPTION_INSCRIPTER_TABLE(scribeType)    \
static Table<scribeType> CreateTable(scribeType& scribe)

#define INSCRIPTION_INSCRIPTER_CREATE_TABLE(scribeType) \
Table<scribeType> INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_INSCRIPTER_RETURN_TABLE \
return INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_INSCRIPTER_DECLARE_SERIALIZE_FUNCTION(scribeType)   \
static void SerializeTable(                                             \
Table<scribeType>& INSCRIPTION_TABLE_SYMBOL,                            \
scribeType& INSCRIPTION_SCRIBE_SYMBOL,                                  \
ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_SERIALIZE_FUNCTION(type, scribeType)  \
void Inscripter<type>::SerializeTable(                                      \
Table<scribeType>& INSCRIPTION_TABLE_SYMBOL,                                \
scribeType& INSCRIPTION_SCRIBE_SYMBOL,                                      \
ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_SERIALIZE_FUNCTION(scribeType)   \
static void SerializeTable(                                     \
Table<scribeType>& INSCRIPTION_TABLE_SYMBOL,                    \
scribeType& INSCRIPTION_SCRIBE_SYMBOL,                          \
ManagedT& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_CALL_BASE_SERIALIZE_FUNCTION                                                                 \
InscripterBase<ManagedT>::SerializeTable(INSCRIPTION_TABLE_SYMBOL, INSCRIPTION_SCRIBE_SYMBOL, INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DECLARE_CONSTRUCT_FUNCTION(scribeType)                               \
static void ConstructTable(Table<scribeType>& INSCRIPTION_TABLE_SYMBOL, scribeType& INSCRIPTION_SCRIBE_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_CONSTRUCT_FUNCTION(type, scribeType)                                      \
void Inscripter<type>::ConstructTable(Table<scribeType>& INSCRIPTION_TABLE_SYMBOL, scribeType& INSCRIPTION_SCRIBE_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DECLARE_CONSTRUCT_OBJECT_FUNCTION(scribeType)                                    \
static void ConstructObjectInstance(scribeType& INSCRIPTION_SCRIBE_SYMBOL, ManagedT*& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_DEFINE_CONSTRUCT_OBJECT_FUNCTION(type, scribeType)                                           \
void Inscripter<type>::ConstructObjectInstance(scribeType& INSCRIPTION_SCRIBE_SYMBOL, ManagedT*& INSCRIPTION_OBJECT_SYMBOL)

#define INSCRIPTION_INSCRIPTER_CALL_TABLE_CONSTRUCTOR       \
obj = new ManagedT(MakeAndConstructTable<ManagedT>(scribe))

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
#define INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER(scribeType)             \
static ::Inscription::ClassNameResolver<scribeType> classNameResolver;

#define INSCRIPTION_DEFINE_CLASS_NAME_RESOLVER(type, scribeType)                \
::Inscription::ClassNameResolver<scribeType> Inscripter<type>::classNameResolver

#define INSCRIPTION_CLASS_NAME_RESOLVER(scribeType)                     \
static ::Inscription::ClassNameResolver<scribeType> classNameResolver

#define INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(type, defaultName, scribeType)                    \
::Inscription::ClassNameResolver<scribeType> Inscripter<type>::classNameResolver([](scribeType& scribe) \
{                                                                                                       \
    return defaultName;                                                                                 \
});

////////////////////////
// SERIALIZATION DEFINES
#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION void INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION(scribeType) void Serialize(scribeType& INSCRIPTION_SCRIBE_SYMBOL)
#endif

#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION_DECLARE void INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL);
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION_DECLARE(scribeType) void Serialize(scribeType& INSCRIPTION_SCRIBE_SYMBOL);
#endif

#ifdef INSCRIPTION_SERIALIZE_SYMBOL
    #define INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(type) void type::INSCRIPTION_SERIALIZE_SYMBOL(::Inscription::Scribe& INSCRIPTION_SCRIBE_SYMBOL)
#else
    #define INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(type, scribeType) void type::Serialize(scribeType& INSCRIPTION_SCRIBE_SYMBOL)
#endif

////////////////
// TABLE DEFINES
#define INSCRIPTION_TABLE_CONSTRUCTOR_DECLARE(type, scribeType)    \
type(const ::Inscription::Table<scribeType, type>& table)

#define INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE(type, scribeType)    \
type::type(const ::Inscription::Table<scribeType, type>& table)

#define INSCRIPTION_TABLE_CONSTRUCTOR_DEFINE_TEMPLATE(type, realType, scribeType)    \
realType::type(const ::Inscription::Table<scribeType, type>& table)

#define INSCRIPTION_TABLE_CONSTRUCTOR(type, scribeType)    \
type(const ::Inscription::Table<scribeType, type>& table)

#define INSCRIPTION_TABLE_BEGIN(scribeType)                         \
static TableT CreateTable(scribeType& INSCRIPTION_SCRIBE_SYMBOL)    \
{                                                                   \
    TableT INSCRIPTION_TABLE_SYMBOL;

#define INSCRIPTION_TABLE_END           \
    return INSCRIPTION_TABLE_SYMBOL;    \
}

#define INSCRIPTION_NULL_TABLE(scribeType)  \
INSCRIPTION_TABLE_BEGIN(scribeType)         \
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
#define INSCRIPTION_TABLE_ADD(var)                                                      \
INSCRIPTION_TABLE_SYMBOL.Add<                                                           \
decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var), \
& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
// Adds the class variable into the table. Will call BinaryScribe::OwningPointer family of functions
#define INSCRIPTION_TABLE_ADD_OWNING_POINTER(var)                                       \
INSCRIPTION_TABLE_SYMBOL.AddOwningPointer<                                              \
decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var), \
& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
// Adds the class variable into the table. Will call BinaryScribe::UnowningPointer family of functions
#define INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(var)                                     \
INSCRIPTION_TABLE_SYMBOL.AddUnowningPointer<                                            \
decltype(std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var), \
& std::remove_reference<decltype(INSCRIPTION_TABLE_SYMBOL)>::type::ClassT::var>(#var);
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
#define INSCRIPTION_TABLE_GET_MEM(var)  \
var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)))
// Forces a copy out of the table instead of moving
#define INSCRIPTION_TABLE_GET_MEM_COPY(var) \
var(INSCRIPTION_TABLE_GET(var))
// Extracts the constant argument from the table
#define INSCRIPTION_TABLE_GET_MEM_CONST(var)    \
var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_const<decltype(var)>::type>(#var)))
// Defines arguments before the get call
#define INSCRIPTION_TABLE_GET_MEM_PRE(var, ...) \
var(__VA_ARGS__, std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)))
// Defines arguments after the get call
#define INSCRIPTION_TABLE_GET_MEM_POST(var, ...)    \
var(std::move(INSCRIPTION_TABLE_SYMBOL.Get<std::remove_reference<decltype(var)>::type>(#var)), __VA_ARGS__)
// Sets up the base class from the table
#define INSCRIPTION_TABLE_GET_BASE(baseT)   \
baseT(INSCRIPTION_TABLE_SYMBOL.Convert<baseT>())

#define INSCRIPTION_TABLE_ASSIGN(var) var = INSCRIPTION_TABLE_SYMBOL.Get<decltype(var)>(#var)