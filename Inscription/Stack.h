#pragma once

#include <stack>

#include "BinaryScribe.h"

#include "ContainerSize.h"
#include "ScopedConstructor.h"

namespace Inscription
{
    namespace detail
    {
        template<class T, class Container>
        struct StackSaver : public std::stack<T, Container>
        {
            void operator()(OutputBinaryScribe& scribe)
            {
                ContainerSize size(c.size());
                scribe.Save(size);
                for(auto loop = c.begin(); loop != c.end(); ++loop)
                    scribe.Save(*loop);
            }
        };

        template<class T, class Container>
        struct StackLoader : public std::stack<T, Container>
        {
            void operator()(InputBinaryScribe& scribe)
            {
                typedef std::stack<T, Container> ContainerT;

                ContainerSize size;
                scribe.Load(size);

                c.clear();
                while(size-- > 0)
                {
                    ScopedConstructor<typename ContainerT::value_type> constructor(scribe);
                    push(std::move(constructor.GetMove()));
                    scribe.ReplaceTrackedObject(*constructor.Get(), c.back());
                }
            }
        };
    }

    template<class T, class Container>
    void Save(OutputBinaryScribe& scribe, std::stack<T, Container>& obj)
    {
        static_cast<detail::StackSaver<T, Container>&>(obj)(scribe);
    }

    template<class T, class Container>
    void Load(InputBinaryScribe& scribe, std::stack<T, Container>& obj)
    {
        static_cast<detail::StackLoader<T, Container>&>(obj)(scribe);
    }

    template<class T, class Container>
    void Serialize(BinaryScribe& scribe, std::stack<T, Container>& obj)
    {
        (scribe.IsOutput()) ? Save(*scribe.AsOutput(), obj) : Load(*scribe.AsInput(), obj);
    }
}