#pragma once

#include "ConstructDispatch.h"

#define INSCRIPTION_ACCESS                  \
friend ::Inscription::ConstructDispatch;    \
template<class Object>                      \
friend class ::Inscription::Scribe

#define INSCRIPTION_TABLE_ACCESS        \
INSCRIPTION_ACCESS;                     \
template<class Object, class Archive>   \
friend struct ::Inscription::TableData