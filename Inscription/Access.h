#pragma once

#define INSCRIPTION_ACCESS              \
template<class Object, class Archive>   \
friend class ::Inscription::Scribe;