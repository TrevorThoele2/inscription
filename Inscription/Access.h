#pragma once

#define INSCRIPTION_ACCESS                                          \
template<class Object, class Archive>                               \
friend class ::Inscription::Scribe;                                 \
template<class Object, class Archive>                               \
friend class ::Inscription::ScribeBase;                             \
template<class Object, class Archive>                               \
friend class ::Inscription::CompositeScribe;                        \
template<class T, class... Args>                                    \
friend struct ::Chroma::is_braces_constructible;                    \
template<class T, class... Args>                                    \
friend struct ::Chroma::is_braces_default_constructible;            \
template<class T, class... Args>                                    \
friend auto ::Chroma::detail::is_braces_constructible_impl(int) ->  \
decltype(void(T{ std::declval<Args>()... }), std::true_type())

#define INSCRIPTION_TABLE_ACCESS                                    \
template<class Object, class Archive>                               \
friend class ::Inscription::Scribe;                                 \
template<class Object, class Archive>                               \
friend struct ::Inscription::TableData;                             \
template<class Table, class Object, class Archive>                  \
friend class ::Inscription::AutoTableDataLink;                      \
template<class Object, class Archive>                               \
friend class ::Inscription::TableBase;                              \
template<class T, class... Args>                                    \
friend struct ::Chroma::is_braces_constructible;                    \
template<class T, class... Args>                                    \
friend struct ::Chroma::is_braces_default_constructible;            \
template<class T, class... Args>                                    \
friend auto ::Chroma::detail::is_braces_constructible_impl(int) ->  \
decltype(void(T{ std::declval<Args>()... }), std::true_type())