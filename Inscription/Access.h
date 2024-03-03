#pragma once

#include "ConstructDispatch.h"

#define INSCRIPTION_ACCESS                  \
friend ::Inscription::ConstructDispatch;    \
template<class Object>                      \
friend class ::Inscription::Scribe;         \
template<class Object, class Format>        \
friend struct ::Inscription::ScribeTraits

#define INSCRIPTION_TABLE_ACCESS        \
INSCRIPTION_ACCESS;                     \
template<class Object, class Format>   \
friend struct ::Inscription::TableData