#pragma once

namespace Inscription
{
    template<class Left, class Right>
    void Assign(Left &lhs, const Right &rhs)
    {
        lhs = rhs;
    }
}