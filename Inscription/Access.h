#pragma once

#define INSCRIPTION_ACCESS                          \
template<class Object, class Archive>               \
friend class ::Inscription::Scribe

#define INSCRIPTION_TABLE_ACCESS                    \
template<class Object, class Archive>               \
friend class ::Inscription::Scribe;                 \
template<class Object, class Archive>               \
friend struct ::Inscription::TableData;             \
template<class Table, class Object, class Archive>  \
friend class ::Inscription::AutoTableDataEntry