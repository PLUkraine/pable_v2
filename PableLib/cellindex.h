#pragma once

#include <stdint.h>
#include <QDebug>

namespace Pable {

using IntType = int32_t;

struct CellIndex
{
    IntType row;
    IntType col;
};

QDebug operator<<(QDebug dbg, const CellIndex &index);

} // namespace Pable
